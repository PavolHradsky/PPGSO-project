//
// Created by peter on 26. 11. 2022.
//

#include "LightHouse.h"
#include "scene.h"
#include "PerlinNoise.h"
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include "shaders/my_texture_frag_glsl.h"
#include "shaders/diffuse_vert_glsl.h"
#include "shaders/diffuse_frag_glsl.h"
#include "shaders/phong_vert_glsl.h"
#include "shaders/phong_frag_glsl.h"
// shared resources
std::unique_ptr<ppgso::Mesh> LightHouse::mesh;
std::unique_ptr<ppgso::Texture> LightHouse::texture;
std::unique_ptr<ppgso::Shader> LightHouse::shader;

LightHouse::LightHouse() {
    // Scale the default model
    scale *= 0.05f;
    position.y = -79;
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("lightHouse.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("lightHouse.obj");
}


bool LightHouse::update(Scene &scene, float dt) {

    generateModelMatrix();
    return true;
}

void LightHouse::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("CamPos", scene.camera->position);
    shader->setUniform("global_lighting_on", scene.global_lighting_on);

    shader->setUniform("MaterialShininess", .0f);
    shader->setUniform("MaterialAmbient",{ 0.9f, 0.9f, 0.9f});
    shader->setUniform("MaterialDiffuse", {0.5f, 0.5f, 0.5f});
    shader->setUniform("MaterialSpecular", {0.5f, 0.5f, 0.5f});

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

void LightHouse::onClick(Scene &scene) {
    std::cout << "Ocean has been clicked!" << std::endl;
}
