//
// Created by peter on 26. 11. 2022.
//

#include "LightHouse.h"
#include "scene.h"
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

#include <cmath>
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
    age += dt;
    //rotate lightdirection
//    scene.lights.positions[0].x += 5.0f * dt;

    // rotate light around the lighthouse
    scene.lights.positions[0].x = position.x + 15 * sin(age);
    scene.lights.positions[0].z = position.z + 15 * cos(age);
//    shader->setUniform("lights.positions[0]",scene.lights.positions[0].x);
    generateModelMatrix();
    return true;
}

void LightHouse::render(Scene &scene) {
    shader->use();

    // Set up light
    //shader->setUniform("LightDirection", scene.lightDirection);
    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("CamPos", scene.camera->position);
    //shader->setUniform("global_lighting_on", scene.global_lighting_on);

    shader->setUniform("material.ambient", {1, 1, 1});
    shader->setUniform("material.diffuse", {1, 1, 1});
    //shader->setUniform("material.specular", {0.9f, 0.9f, 0.9f});
    shader->setUniform("material.shininess", 32.0f);

    for (int i = 0; i < scene.lights.count; i++) {
        shader->setUniform("lights.positions[" + std::to_string(i) + "]", scene.lights.positions[i]);
        shader->setUniform("lights.colors[" + std::to_string(i) + "]", scene.lights.colors[i]);
        shader->setUniform("lights.ranges[" + std::to_string(i) + "]", scene.lights.ranges[i]);
        if (scene.lights.strengths[i] < 0) {
            shader->setUniform("lights.strengths[" + std::to_string(i) + "]", 0.0f);
        }
        else {
            shader->setUniform("lights.strengths[" + std::to_string(i) + "]", scene.lights.strengths[i]);
        }
    }

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    shader->setUniform("UseShadow", false);
    mesh->render();
}
