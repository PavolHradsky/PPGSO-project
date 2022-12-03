//
// Created by peter on 26. 11. 2022.
//

#include "Bubble.h"
#include <glm/gtc/random.hpp>
#include "Ocean.h"
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include <glm/gtx/euler_angles.hpp>

// TODO od Plevka, prerobit, sfunkcnit




// Static resources
std::unique_ptr<ppgso::Mesh> Bubble::mesh;
std::unique_ptr<ppgso::Texture> Bubble::texture;
std::unique_ptr<ppgso::Shader> Bubble::shader;

Bubble::Bubble(glm::mat4 translateMatrix, float timeAlive, float rndPos) {
    auto rndmPos = random_vec3(-rndPos, 0);
    translateMatrix[3][0] += rndmPos.x;
    translateMatrix[3][1] += rndmPos.y;
    translateMatrix[3][2] += rndmPos.z;
    this->translateMatrix = translateMatrix;

    rotation = random_vec3(-0.1, 0.1);
    scale = random_vec3(minScale, maxScale);

    color = {0.6,0.85,0.92};

    time = timeAlive;
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
}

glm::vec3 Bubble::random_vec3 (float mini, float maxi) {
    return {((float) rand() / (float) RAND_MAX) * (maxi - mini) + mini, ((float) rand() / (float) RAND_MAX) * (maxi - mini) + mini, ((float) rand() / (float) RAND_MAX) * (maxi - mini) + mini};
}

bool Bubble::update(Scene &scene, float dt) {
    auto speed = random_vec3(0.005, 0.0005);

    translateMatrix[3][0] += scene.camera->position.x;
    translateMatrix[3][1] += speed.y;
    translateMatrix[3][2] += scene.camera->position.z;

    if (time < 0)
        return false;

    time--;
    modelMatrix =
            translateMatrix
            * glm::orientate4(rotation)
            * glm::scale(glm::mat4(1.0f), scale);

    return true;
}

void Bubble::render(Scene &scene) {
    shader->use();

    shader->setUniform("LightDirection", scene.lightDirection);

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("OverallColor", color);
    mesh->render();
}

