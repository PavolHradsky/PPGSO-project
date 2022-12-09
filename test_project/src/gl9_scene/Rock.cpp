#include "Rock.h"
#include "scene.h"
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include "shaders/my_texture_frag_glsl.h"
#include "shaders/diffuse_vert_glsl.h"
#include "shaders/diffuse_frag_glsl.h"
#include "shaders/phong_vert_glsl.h"
#include "shaders/phong_frag_glsl.h"
// shared resources
std::unique_ptr<ppgso::Mesh> Rock::mesh;
std::unique_ptr<ppgso::Texture> Rock::texture;
std::unique_ptr<ppgso::Shader> Rock::shader;

Rock::Rock() {
    // Scale the default model
    scale *= 0.05f;
    position.y = -79;
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Rock.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Rock.obj");

}


bool Rock::update(Scene &scene, float dt) {

    generateModelMatrix();
    return true;
}

void Rock::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", {1.0f, -1.0f, -1.0f});

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("CamPos", scene.camera->position);
    shader->setUniform("global_lighting_on", scene.global_lighting_on);
    // rotate light



    shader->setUniform("material.ambient", {1, 1, 1});
    shader->setUniform("material.diffuse", {1, 1, 1});
    shader->setUniform("material.specular", {0.9f, 0.9f, 0.9f});
    shader->setUniform("material.shininess", 32.0f);
    for (int i = 0; i < scene.lights.count; i++) {

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
    shader->setUniform("UseShadow", false);
    mesh->render();
}
