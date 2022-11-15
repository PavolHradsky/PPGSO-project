//
// Created by peter on 21. 9. 2022.
//
#include "Missile.h"
#include "Scene.h"

#include "shaders/texture_vert_glsl.h"
#include "shaders/texture_frag_glsl.h"
#include "shaders/diffuse_vert_glsl.h"
#include "shaders/diffuse_frag_glsl.h"

Missile::Missile() {
    // Initialize static resources if needed
    glm::mat4 modelMatrix{1.0f};
    glm::vec3 position{0, 0, 0};
    glm::vec3 scale{1, 1, 1};


    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("models/missile.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("models/missile.obj");
}

bool Missile::update(Scene &scene, float dt) {
//    modelMatrix = glm::mat4{1.0f};
    generateModelMatrix();

    return true;
}

void Missile::render(Scene &scene) {
//    glDepthMask(GL_FALSE);

    shader->use();


    //shader->setUniform("TextureOffset", offset);
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("LightDirection", scene.lightDirection);
//    shader->setUniform("ViewMatrix", glm::mat4{1.0f});
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
//    shader->setUniform("ProjectionMatrix", glm::mat4{1.0f});
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
//    glDepthMask(GL_TRUE);
}

// shared resources
std::unique_ptr<ppgso::Mesh> Missile::mesh;
std::unique_ptr<ppgso::Shader> Missile::shader;
std::unique_ptr<ppgso::Texture> Missile::texture;
