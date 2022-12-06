#include "Treasure.h"
#include "scene.h"
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
// shared resources
std::unique_ptr<ppgso::Shader> Treasure::shader;
std::unique_ptr<ppgso::Mesh> Treasure::mesh;
std::unique_ptr<ppgso::Texture> Treasure::texture;

Treasure::Treasure() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sphere.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("ChestCartoon.obj");
}

bool Treasure::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Treasure::render(Scene &scene) {
    shader->use();
    shader->setUniform("Transparency", 0);
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