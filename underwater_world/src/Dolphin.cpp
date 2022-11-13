//
// Created by peter on 21. 9. 2022.
//
#include "Dolphin.h"
#include "Scene.h"

#include "shaders/texture_vert_glsl.h"
#include "shaders/texture_frag_glsl.h"

Dolphin::Dolphin() {

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("models/test.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("models/dolphin.obj");
}

bool Dolphin::update(Scene &scene, float dt) {
//    offset.y -= dt/5;

    generateModelMatrix();
    return true;
}

void Dolphin::render(Scene &scene) {
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
std::unique_ptr<ppgso::Mesh> Dolphin::mesh;
std::unique_ptr<ppgso::Shader> Dolphin::shader;
std::unique_ptr<ppgso::Texture> Dolphin::texture;
