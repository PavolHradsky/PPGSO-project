//
// Created by peter on 25. 11. 2022.
//

#include "Sand.h"
#include "scene.h"
#include "PerlinNoise.h"
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
// shared resources
std::unique_ptr<ppgso::Mesh> Sand::mesh;
std::unique_ptr<ppgso::Texture> Sand::texture;
std::unique_ptr<ppgso::Shader> Sand::shader;

Sand::Sand() {
    // Scale the default model
    scale *= 20.0f;
    position = {0,-80,0};
    // TODO urobit funkcne perlin noise... chyba je ze mesh->vertices.size() je stale 1 a to je asi malo, pretoze object mame scalnuty a nemame viac vertexov, treba asi ist cez bezier surface

    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Sand.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Ocean.obj");

}


bool Sand::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Sand::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

void Sand::onClick(Scene &scene) {
    std::cout << "Ocean has been clicked!" << std::endl;
}
