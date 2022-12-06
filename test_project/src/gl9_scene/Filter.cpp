#include "Filter.h"
#include "scene.h"
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
// shared resources
std::unique_ptr<ppgso::Shader> Filter::shader;
std::unique_ptr<ppgso::Mesh> Filter::mesh;
std::unique_ptr<ppgso::Texture> Filter::texture;

Filter::Filter() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("stars.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("filter.obj");
}


bool Filter::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Filter::render(Scene &scene) {
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