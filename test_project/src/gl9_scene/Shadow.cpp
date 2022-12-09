//
// Created by hrads on 9. 12. 2022.
//
#include "Shadow.h"
#include "Ocean.h"
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include <cmath>
#include <glm/gtx/euler_angles.hpp>

// Static resources
std::unique_ptr<ppgso::Mesh> Shadow::mesh;
std::unique_ptr<ppgso::Texture> Shadow::texture;
std::unique_ptr<ppgso::Shader> Shadow::shader;


Shadow::Shadow() {
    color = {0.1, 0.1, 0.1};
    scale *= 0.07;
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("shadow.obj");

}

bool Shadow::update(Scene &scene, float dt) {

    generateModelMatrix();
    return true;
}

void Shadow::render(Scene &scene) {
    shader->use();

    shader->setUniform("LightDirection", scene.lightDirection);

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("OverallColor", color);
    mesh->render();
}

