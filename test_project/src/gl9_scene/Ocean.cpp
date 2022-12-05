#include "Ocean.h"
#include "scene.h"
#include "Boat.h"
#include "Dolphin.h"
#include "explosion.h"
#include "PerlinNoise.h"
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
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
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Ocean.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Ocean.obj");
}


bool Ocean::update(Scene &scene, float dt) {
    if(prevCamY > 0 && scene.camera->position.y < 0){
        shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, my_texture_frag_glsl);
    }
    if(prevCamY < 0 && scene.camera->position.y > 0){
        shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
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

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

void Ocean::onClick(Scene &scene) {
    std::cout << "Ocean has been clicked!" << std::endl;
}
