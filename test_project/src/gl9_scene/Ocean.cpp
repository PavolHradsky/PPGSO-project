#include "Ocean.h"
#include "scene.h"
#include "Boat.h"
#include "Dolphin.h"
#include "explosion.h"
#include "PerlinNoise.h"
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>
#include "shaders/my_phong_frag_glsl.h"
#include "shaders/my_texture_frag_glsl.h"
#define SEA_TURBULENCE 0.00f
// shared resources
std::unique_ptr<ppgso::Mesh> Ocean::mesh;
std::unique_ptr<ppgso::Texture> Ocean::texture;
std::unique_ptr<ppgso::Shader> Ocean::shader;

Ocean::Ocean() {
    // Scale the default model
    scale *= 20.0f;
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Ocean.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Ocean.obj");
}


bool Ocean::update(Scene &scene, float dt) {
    if(prevCamY > 0 && scene.camera->position.y < 0){
        shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, my_phong_frag_glsl);
    }
    if(prevCamY < 0 && scene.camera->position.y > 0){
        shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    }
    prevCamY = scene.camera->position.y;

    generateModelMatrix();
    return true;
}

void Ocean::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("CamPos", scene.camera->position);
    shader->setUniform("global_lighting_on", scene.global_lighting_on);

//    shader->setUniform("material.ambient", {1, 1, 1});
    shader->setUniform("material.diffuse", {1, 1, 1});
    shader->setUniform("material.specular", {0.9f, 0.9f, 0.9f});
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

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    shader->setUniform("UseShadow", false);
    mesh->render();
}

void Ocean::onClick(Scene &scene) {
    std::cout << "Ocean has been clicked!" << std::endl;
}
