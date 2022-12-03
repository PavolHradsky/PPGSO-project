//
// Created by peter on 3. 12. 2022.
//

#include "Seaweed.h"
#include "scene.h"
#include "shaders/texture_vert_glsl.h"
#include "shaders/texture_frag_glsl.h"
// Static resources
std::unique_ptr<ppgso::Mesh> Seaweed::mesh;
std::unique_ptr<ppgso::Texture> Seaweed::texture;
std::unique_ptr<ppgso::Shader> Seaweed::shader;

Seaweed::Seaweed() {

    scale *= 3;
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sphere.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("SeaWeed.obj");
}


bool Seaweed::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Seaweed::render(Scene &scene) {
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



