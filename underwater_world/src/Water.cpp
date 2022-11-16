//
// Created by peter on 21. 9. 2022.
//
#include "Water.h"
#include "Scene.h"

#include "shaders/texture_vert_glsl.h"
#include "shaders/texture_frag_glsl.h"

Water::Water() {
    // Initialize static resources if needed
    //glm::mat4 modelMatrix{1.0f};
    glm::vec3 position{-20, -20, -40};
    glm::vec3 scale{12, 40, 0};


    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("models/corsair.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("models/missile.obj");
}

bool Water::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Water::render(Scene &scene) {
    //glDepthMask(GL_FALSE);

    shader->use();

    shader->setUniform("TextureOffset", offset);
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
    //glDepthMask(GL_TRUE);
}

// shared resources
std::unique_ptr<ppgso::Mesh> Water::mesh;
std::unique_ptr<ppgso::Shader> Water::shader;
std::unique_ptr<ppgso::Texture> Water::texture;
