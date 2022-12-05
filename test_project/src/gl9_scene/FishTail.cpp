//
// Created by peter on 29. 11. 2022.
//

#include "FishTail.h"
#include "scene.h"
#include "FishTail.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include "shaders/my_texture_frag_glsl.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

#define SEA_TURBULENCE 0.00f
// shared resources
std::unique_ptr<ppgso::Mesh> FishTail::mesh;
std::unique_ptr<ppgso::Texture> FishTail::texture;
std::unique_ptr<ppgso::Shader> FishTail::shader;

FishTail::FishTail() {
    // Scale the default model
    //scale *= glm::linearRand(1, 5);
    //offset = 0.2891;
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("fish.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("fishTail.obj");
}

void FishTail::updateTail(Scene &scene, float posX, float posY, float posZ, float speed, float radius, glm::vec3 pos, glm::vec3 sc, float age, float dt) {
    scale = sc;
    // move in circle
    position.x = std::cos(age * speed) * radius + posX;
    position.z = std::sin(age * speed) * radius + posZ;
    position.y = pos.y;

    float nextX = std::cos((age + dt) * speed) * radius + posX;
    float nextZ = std::sin((age + dt) * speed) * radius + posZ;

    // rotate fish
    rotation.z = std::atan2(nextX - position.x, nextZ - position.z) + ppgso::PI / 2 + sin(age*3)*0.5;

    // TODO
    // if fish is tooclose to 118, 0, 0, rotate it to the other side
    if (glm::distance(position, glm::vec3(118, 0, 0)) < 10) {
        rotation.z = ppgso::PI;
    }
    if (glm::distance(position, glm::vec3(0, 0, 118)) < 10) {
        rotation.z = ppgso::PI;
    }
    update(scene, 0);

}

bool FishTail::update(Scene &scene, float dt) {

    generateModelMatrix();
    return true;
}

void FishTail::render(Scene &scene) {

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

void FishTail::onClick(Scene &scene) {
    std::cout << "Fish has been clicked!" << std::endl;
}
