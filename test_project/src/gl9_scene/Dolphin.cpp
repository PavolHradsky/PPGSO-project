#include <glm/gtc/random.hpp>
#include "scene.h"
#include "Dolphin.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Dolphin::mesh;
std::unique_ptr<ppgso::Shader> Dolphin::shader;
std::unique_ptr<ppgso::Texture> Dolphin::texture;

Dolphin::Dolphin(glm::vec3 pos, glm::vec3 rot, float freq) {
    position = pos;
    rotation = rot;
    frequency = freq;

    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("dolphin.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("dolphin.obj");
}

bool Dolphin::update(Scene &scene, float dt) {
    // Accelerate
    //speed += glm::vec3{0.0f, 20.0f, 0.0f} * dt;

    position.x = position.x;
    position.y = position.y ;
    position +=position;
    rotation_ += frequency;
    // Move the projectile
    rotation.x = rotation_;

    generateModelMatrix();
    return true;
}

void Dolphin::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    shader->setUniform("material.ambient", {0.05f, 0.05f, 0.05f});
    shader->setUniform("material.diffuse", {0.8f, 0.8f, 0.8f});
    shader->setUniform("material.specular", {0.9f, 0.9f, 0.9f});
    shader->setUniform("material.shininess", 32.0f);
    for (int i = 0; i < 1; i++) {
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
    mesh->render();
}
