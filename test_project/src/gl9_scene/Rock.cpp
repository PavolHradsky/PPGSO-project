//
// Created by peter on 26. 11. 2022.
//

#include "Rock.h"
#include "scene.h"
#include "PerlinNoise.h"
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include "shaders/my_texture_frag_glsl.h"
// shared resources
std::unique_ptr<ppgso::Mesh> Rock::mesh;
std::unique_ptr<ppgso::Texture> Rock::texture;
std::unique_ptr<ppgso::Shader> Rock::shader;

Rock::Rock() {
    // Scale the default model
    scale *= 0.05f;
    position.y = -79;
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Rock.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Rock.obj");

}


bool Rock::update(Scene &scene, float dt) {
//    if(prevCamY > 0 && scene.camera->position.y < 0){
//        shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, my_texture_frag_glsl);
//    }
//    if(prevCamY < 0 && scene.camera->position.y > 0){
//        shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
//    }
//    prevCamY = scene.camera->position.y;

    generateModelMatrix();
    return true;
}

void Rock::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("CamPos", scene.camera->position);
    shader->setUniform("global_lighting_on", scene.global_lighting_on);

    shader->setUniform("MaterialShininess", 12.0f);
    shader->setUniform("MaterialAmbient",{ 0.1f, 0.1f, 0.1f});
    shader->setUniform("MaterialDiffuse", {0.5f, 0.5f, 0.5f});
    shader->setUniform("MaterialSpecular", {0.5f, 0.5f, 0.5f});
    // TODO nastavit tiene kamenu
    // lights count
    shader->setUniform("LightCount", scene.rock_lights+1);
    for (int i = 0; i < scene.rock_lights+1; i++) {
        shader->setUniform("lights.positions[" + std::to_string(i) + "]", scene.lights.positions[i]);
        shader->setUniform("lights.colors[" + std::to_string(i) + "]", scene.lights.colors[i]);
        shader->setUniform("lights.ranges[" + std::to_string(i) + "]", scene.lights.ranges[i]);
        if (scene.lights.strengths[i] < 0) {
            shader->setUniform("lights.strengths[" + std::to_string(i) + "]", 5.0f);
        }
        else {
            shader->setUniform("lights.strengths[" + std::to_string(i) + "]", scene.lights.strengths[i]);
        }
    }
    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

void Rock::onClick(Scene &scene) {
    std::cout << "Ocean has been clicked!" << std::endl;
}
