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

void FishTail::updateTail(Scene &scene, glm::vec3 pos, glm::vec3 rot, glm::vec3 sc) {
    scale = sc;

    // make ofset for tail

    //offset = {(distanceZ + distanceX) * sin(rot.y), distanceY, (distanceZ+di) * cos(rot.y)};
    offset = {distanceX * sin(rot.y), distanceY, distanceZ * cos(rot.y)};
    position = pos + offset;

    rotation = rot;

    update(scene, 0);

}

bool FishTail::update(Scene &scene, float dt) {

    //rotation.y += dt;

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
