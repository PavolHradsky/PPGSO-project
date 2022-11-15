//
// Created by peter on 21. 9. 2022.
//
#include "Missile.h"
#include "Scene.h"

#include "shaders/texture_vert_glsl.h"
#include "shaders/texture_frag_glsl.h"

Missile::Missile() {
    // Initialize static resources if needed
    glm::mat4 modelMatrix{1.0f};
    glm::vec3 position{20, 25, -40};
    glm::vec3 scale{120, 40, 0};


    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("models/missile.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("models/missile.obj");
}

bool Missile::update(Scene &scene, float dt) {
    modelMatrix = glm::mat4{1.0f};
    generateModelMatrix();
    return true;
}

void Missile::render(Scene &scene) {
    glDepthMask(GL_FALSE);

    shader->use();

    shader->setUniform("TextureOffset", offset);
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", glm::mat4{1.0f});
    shader->setUniform("ProjectionMatrix", glm::mat4{1.0f});
    shader->setUniform("Texture", *texture);
    mesh->render();
    glDepthMask(GL_TRUE);
}

// shared resources
std::unique_ptr<ppgso::Mesh> Missile::mesh;
std::unique_ptr<ppgso::Shader> Missile::shader;
std::unique_ptr<ppgso::Texture> Missile::texture;
