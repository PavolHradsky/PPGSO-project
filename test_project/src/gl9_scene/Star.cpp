//
// Created by hrads on 7. 12. 2022.
//
#include "Star.h"
#include "scene.h"
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>
#include "shaders/my_phong_frag_glsl.h"
#include "shaders/my_texture_frag_glsl.h"

// shared resources
std::unique_ptr<ppgso::Mesh> Star::mesh;
std::unique_ptr<ppgso::Texture> Star::texture;
std::unique_ptr<ppgso::Shader> Star::shader;

Star::Star(glm::vec3 pos) {
//    color = {1, 1, 0};
    // Initialize static resources if needed
    scale *= 0.005;
    position = pos + glm::linearRand(-5.0f, 5.0f);
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sun.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("star.obj");
}


bool Star::update(Scene &scene, float dt) {
    age += dt;

    position.y += dt;
    rotation.z += dt;

    if(age > 3){
        return false;
    }

    generateModelMatrix();
    return true;
}

void Star::render(Scene &scene) {
    shader->use();

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
