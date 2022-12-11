#include "Filter.h"
#include "scene.h"
#include <shaders/texture_vert_glsl.h>
#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>
#include "shaders/convolution_frag_glsl.h"
#include "shaders/convolution_vert_glsl.h"

// shared resources
std::unique_ptr<ppgso::Shader> Filter::shader;
std::unique_ptr<ppgso::Mesh> Filter::mesh;
std::unique_ptr<ppgso::Texture> Filter::texture;

Filter::Filter() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sky.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("filter.obj");
}


bool Filter::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Filter::render(Scene &scene) {
    if (scene.convolution && !scene.prevConvolution) {
        shader = std::make_unique<ppgso::Shader>(convolution_vert_glsl, convolution_frag_glsl);
    }
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("CamPos", scene.camera->position);
    shader->setUniform("global_lighting_on", scene.global_lighting_on);

    shader->setUniform("material.diffuse", {0.5, 0.5, 0.5});
    shader->setUniform("material.specular", {0.9f, 0.9f, 0.9f});
    shader->setUniform("material.shininess", 32.0f);

    for (int i = 0; i < scene.lights.count; i++) {
        shader->setUniform("lights.positions[" + std::to_string(i) + "]", scene.lights.positions[i]);
        shader->setUniform("lights.colors[" + std::to_string(i) + "]", scene.lights.colors[i]);
        shader->setUniform("lights.ranges[" + std::to_string(i) + "]", scene.lights.ranges[i]);
        if (scene.lights.strengths[i] < 0) {
            shader->setUniform("lights.strengths[" + std::to_string(i) + "]", 0.0f);
        } else {
            shader->setUniform("lights.strengths[" + std::to_string(i) + "]", scene.lights.strengths[i]);
        }
    }

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    shader->setUniform("UseShadow", false);
    mesh->render();
}