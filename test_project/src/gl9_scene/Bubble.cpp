#include "Bubble.h"
#include "Ocean.h"
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include <cmath>
#include <glm/gtx/euler_angles.hpp>

// Static resources
std::unique_ptr<ppgso::Mesh> Bubble::mesh;
std::unique_ptr<ppgso::Texture> Bubble::texture;
std::unique_ptr<ppgso::Shader> Bubble::shader;

#define G 9.81f

Bubble::Bubble() {
    color = {0.6,0.85,0.92};
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");

    if(scale.x > 1.4){
        speed = 0.66f * std::sqrt(G/(scale.x/2));
    } else if(scale.x > 0.8 ) {
        speed = 0.33f * (scale.x/2) * (scale.x/2) * G/14;
    } else {
        speed = 0.11f * (scale.x/2) * (scale.x/2) * G/14;
    }
}

bool Bubble::update(Scene &scene, float dt) {
    // gravitation
    //position.y += dt * speed * 40;
    //position.x += 10 * dt * speed / scale.x;
    //position.z += 10 * dt * speed / scale.x;

    glm::vec3 vec = {15, 40, 15};

    vec *= dt*speed * scale.x;
    position += vec;

    if(position.y > 0){
        return false;
    }

    generateModelMatrix();
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

