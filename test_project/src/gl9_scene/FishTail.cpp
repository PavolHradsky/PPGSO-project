#include "FishTail.h"
#include "scene.h"
#include "FishTail.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include "shaders/my_texture_frag_glsl.h"
#include "shaders/phong_vert_glsl.h"
#include "shaders/phong_frag_glsl.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include "shaders/convolution_frag_glsl.h"
#include "shaders/convolution_vert_glsl.h"

// shared resources
std::unique_ptr<ppgso::Mesh> FishTail::mesh;
std::unique_ptr<ppgso::Texture> FishTail::texture;
std::unique_ptr<ppgso::Shader> FishTail::shader;

FishTail::FishTail() {
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("fish.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("fishTail.obj");
}

void FishTail::updateTail(Scene &scene, float posX, float posY, float posZ, float speed, float radius, glm::vec3 pos, glm::vec3 sc, float age, float dt) {
    scale = sc;
    // move in circle
    position.x = std::cos(age * speed) * radius + posX;
    position.z = std::sin(age * speed) * radius + posZ;
    position.y = pos.y;

    float nextX = std::cos((age + dt) * speed) * radius + posX;
    float nextZ = std::sin((age + dt) * speed) * radius + posZ;

    // rotate fish
    rotation.z = std::atan2(nextX - position.x, nextZ - position.z) + ppgso::PI / 2 + sin(age*3)*0.5;

    // TODO
    // if fish is tooclose to 118, 0, 0, rotate it to the other side
    if (glm::distance(position, glm::vec3(118, 0, 0)) < 10) {
        rotation.z = ppgso::PI;
    }
    if (glm::distance(position, glm::vec3(0, 0, 118)) < 10) {
        rotation.z = ppgso::PI;
    }
    update(scene, 0);

}

bool FishTail::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void FishTail::render(Scene &scene) {
    if (scene.convolution){
        shader = std::make_unique<ppgso::Shader>(convolution_vert_glsl, convolution_frag_glsl);
    }
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