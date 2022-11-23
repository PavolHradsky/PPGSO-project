//
// Created by hrads on 23. 11. 2022.
//
#include "Fish.h"
#include "scene.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#define SEA_TURBULENCE 0.00f
// shared resources
std::unique_ptr<ppgso::Mesh> Fish::mesh;
std::unique_ptr<ppgso::Texture> Fish::texture;
std::unique_ptr<ppgso::Shader> Fish::shader;

Fish::Fish() {
    // Scale the default model
    scale *= 0.01f;
    rotation.x = -ppgso::PI / 4;
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Dolphin.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Dolphin.obj");
}


bool Fish::update(Scene &scene, float dt) {

    age += dt;

    position.y = std::cos(age * speed) * radius;
    position.z = std::sin(age * speed) * radius;

    rotation.x += dt*speed;

    generateModelMatrix();
    return true;
}

void Fish::render(Scene &scene) {
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

void Fish::onClick(Scene &scene) {
    std::cout << "Fish has been clicked!" << std::endl;
}
