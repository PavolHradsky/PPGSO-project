//
// Created by hrads on 23. 11. 2022.
//
#include "Fish.h"
#include "scene.h"
#include "FishTail.h"
#include "Bubble.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include "shaders/my_texture_frag_glsl.h"
#include "shaders/phong_vert_glsl.h"
#include "shaders/phong_frag_glsl.h"
#include "shaders/convolution_frag_glsl.h"
#include "shaders/convolution_vert_glsl.h"

#define SEA_TURBULENCE 0.00f
// shared resources
std::unique_ptr<ppgso::Mesh> Fish::mesh;
std::unique_ptr<ppgso::Texture> Fish::texture;
std::unique_ptr<ppgso::Shader> Fish::shader;

Fish::Fish() {
    // Scale the default model
    scale *= glm::linearRand(1, 5);
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("fish.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("fishBody.obj");
    auto part = std::make_unique<FishTail>();
    tails.push_back(move(part));
}


bool Fish::update(Scene &scene, float dt) {

    if(my_fish)
        scale = glm::vec3(4, 4, 4);

    auto tmp = this->position;
    tmp.z += 0.02f;
    tmp.y += 0.6f;
    dt = animate ? (float) dt : 0;
    age += dt;
    // move of the fish


//    position.z += speed * dt * direction;
//    position.y = sin(age * speed) * radius + posY;
//    position.x = cos(age * speed) * radius + posX;

//     move in circle

    position.x = std::cos(age * speed) * radius + posX;
    position.z = std::sin(age * speed) * radius + posZ;
    position.y = std::sin(age) + posY;
    // std::cout << position.x << std::endl;
    float nextX = std::cos((age + dt) * speed) * radius + posX;
    float nextZ = std::sin((age + dt) * speed) * radius + posZ;

    // rotate fish
    rotation.z = std::atan2(nextX - position.x, nextZ - position.z) + ppgso::PI / 2;

    // TODO
    // if fish is too close to 118, 0, 0, rotate it to the other side
    if (glm::distance(position, glm::vec3(118, 0, 0)) < 10) {
        rotation.z = ppgso::PI;
    }
    if (glm::distance(position, glm::vec3(0, 0, 118)) < 10) {
        rotation.z = ppgso::PI;
    }

    if(my_fish && scene.camera->enableAnimationFish){
        scene.camera->position = position + glm::vec3{-15, 7, 0};
        scene.camera->rotation = glm::vec3{ppgso::PI/6, 10*ppgso::PI/10, 0};
    }

    generateModelMatrix();

    for ( auto& obj : tails ) {
        auto part = dynamic_cast<FishTail*>(obj.get());
        part->updateTail(scene, posX, posY, posZ, speed, radius, position, scale, age-scale.x*0.09, dt);
    }

    // TODO pohyb viac specificky, pri naraze do inej ryby, nieco spravit, po nejakej dobe ryba uhyne a len spadne na zem
    for (auto &object : scene.objects) {
        // Ignore self in scene
        if (object.get() == this)
            continue;
        // We only need to collide with boats
        auto fish = dynamic_cast<Fish *>(object.get());
        if (!fish)
            continue;

        // Check distance between objects
        auto distance = glm::distance(this->position, fish->position);
        if (distance < 10) {

            while (fish->position.y>=-80){
                fish->position.y -= 1;

                fish->position.x = fish->position.x;
                fish->position.z = fish->position.z;
            }
            if (fish->position.y == -80){
                return false;
            }

        }

    }
    /*
    if (fishTail) {
        fishTail->position = position + glm::vec3{offset, 0, 0};
        std::cout << fishTail->position.x << std::endl;
        fishTail->rotation = rotation;
        fishTail->updateTail(scene, position,rotation,scale);
    }*/
//    for ( auto& obj : tails ) {
//        auto part = dynamic_cast<FishTail*>(obj.get());
//        part->updateTail(scene);
//    }


//    if(position.z > 120){
//        direction = -1;
//        rotation.y = 2*ppgso::PI;
//        rotation.z = -ppgso::PI;
//    }
//    if(position.z < -120){
//        direction = 1;
//        rotation.y = 0;
//    }

    //position.y = std::cos(age * speed) * radius;
    //position.z = std::sin(age * speed) * radius;

    //rotation.x += dt*speed;
    // die after 10 seconds


    return true;
}

void Fish::render(Scene &scene) {
    if (scene.convolution && !scene.prevConvolution){
        shader = std::make_unique<ppgso::Shader>(convolution_vert_glsl, convolution_frag_glsl);
    }
    for ( auto& obj : tails ) {

        // TODO> osvietenie napr delfinych chvostov
        //scene.light_positions.at(1) = obj->position + glm::vec3{2, 2, 2};
        auto part = dynamic_cast<FishTail*>(obj.get());
        part->render(scene);
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
