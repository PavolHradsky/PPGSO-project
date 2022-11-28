//
// Created by peter on 26. 11. 2022.
//

#include "Rain.h"
// make rain
#include <glm/gtc/random.hpp>
#include "scene.h"
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include <glm/gtx/euler_angles.hpp>

// Static resources
std::unique_ptr<ppgso::Mesh> Rain::mesh;
std::unique_ptr<ppgso::Texture> Rain::texture;
std::unique_ptr<ppgso::Shader> Rain::shader;

// TODO rain code z copilota, treba sfunkcnit, momentalne tam je 1 gula, ktora by sa dala zneuzit na slnko a z nej spravit luce osvetlenim... alebo nieco ine
Rain::Rain(glm::vec3 position, float minSc, float maxSc, float rndPos) {
    // Set random scale speed and rotation
    this->position = position+50.0f*glm::vec3(0.0f, 1.0f, 0.0f);
    auto rndmPos = random_vec3(-rndPos, rndPos);
    this->position.x += rndmPos.x;
    this->position.y += rndmPos.y;
    this->position.z += rndmPos.z;

    rotation = random_vec3(-0.1, 0.1);
    scale = sameRandom_vec3(minSc, maxSc);

    color = {1,0,0};

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
}

glm::vec3 Rain::random_vec3 (float mini, float maxi) {
    return {((float) rand() / (float) RAND_MAX) * (maxi - mini) + mini,((float) rand() / (float) RAND_MAX) * (maxi - mini) + mini,  ((float) rand() / (float) RAND_MAX) * (maxi - mini) + mini};
}

glm::vec3 Rain::sameRandom_vec3 (float mini, float maxi) {
    float sur = ((float) rand() / (float) RAND_MAX) * (maxi - mini) + mini;
    return { sur, sur, sur};
}

bool Rain::update(Scene &scene, float dt) {
    auto speed = random_vec3(0.005, 0.005);
    translateMatrix[3][0] += scene.camera->position.x;
    translateMatrix[3][1] += speed.y;
    translateMatrix[3][2] += scene.camera->position.z;

    if (dt < 0)
        return false;

    dt--;


    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();
    //modelMatrix = translateMatrix * glm::eulerAngleYXZ(rotation.x, rotation.y, rotation.z * glm::scale(glm::mat4{1.0f}, scale));
    return true;
}

void Rain::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);
    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Color", color);

    // Render mesh
    mesh->render();
}



