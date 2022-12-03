//
// Created by peter on 26. 11. 2022.
//

#include "Sun.h"
// make rain
#include <glm/gtc/random.hpp>
#include "scene.h"
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/light_frag_glsl.h>
#include <shaders/light_vert_glsl.h>
#include <glm/gtx/euler_angles.hpp>

// Static resources
std::unique_ptr<ppgso::Mesh> Sun::mesh;
std::unique_ptr<ppgso::Texture> Sun::texture;
std::unique_ptr<ppgso::Shader> Sun::shader;

// TODO rain code z copilota, treba sfunkcnit, momentalne tam je 1 gula, ktora by sa dala zneuzit na slnko a z nej spravit luce osvetlenim... alebo nieco ine
Sun::Sun() {

    scale *= 25;

    position.y = 190;
    position.x = 40;
    position.z = 40;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sun.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
}


bool Sun::update(Scene &scene, float dt) {

//    posY = position.y;

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();
    //modelMatrix = translateMatrix * glm::eulerAngleYXZ(rotation.x, rotation.y, rotation.z * glm::scale(glm::mat4{1.0f}, scale));
    return true;
}

void Sun::render(Scene &scene) {
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



