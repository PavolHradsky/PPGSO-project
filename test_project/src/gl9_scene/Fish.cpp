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

#define SEA_TURBULENCE 0.00f
// shared resources
std::unique_ptr<ppgso::Mesh> Fish::mesh;
std::unique_ptr<ppgso::Texture> Fish::texture;
std::unique_ptr<ppgso::Shader> Fish::shader;

Fish::Fish() {
    // Scale the default model
    scale *= glm::linearRand(1, 5);
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("fish.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("fishBody.obj");
    auto part = std::make_unique<FishTail>();
    tails.push_back(move(part));
}


bool Fish::update(Scene &scene, float dt) {
//    if(prevCamY > 0 && scene.camera->position.y < 0){
//        shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, my_texture_frag_glsl);
//    }
//    if(prevCamY < 0 && scene.camera->position.y > 0){
//        shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
//    }
//    prevCamY = scene.camera->position.y;

    auto tmp = this->position;
    tmp.z += 0.02f;
    tmp.y += 0.6f;
//    for (int i = 0; i < 10; ++i) {
//        auto bubble = std::make_unique<Bubble>(glm::translate(glm::mat4(1.0f), tmp), ((float) rand() / (float) RAND_MAX) * (500 - 400) + 400, 50);
//        bubble->minScale = 0.04;
//        bubble->maxScale = 0.06;
//        scene.objects.push_back(move(bubble));
//    }
    age += dt;
    // move of the fish

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
        if (distance < 1) {
            std::cout << "fish died";
           while (fish->position.y>-80){
               fish->position.y -= 1;
               fish->position.x = fish->position.x;
               fish->position.z = fish->position.z;
           }
        }

    }
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
    generateModelMatrix();

    for ( auto& obj : tails ) {
        auto part = dynamic_cast<FishTail*>(obj.get());
        part->updateTail(scene, posX, posY, posZ, speed, radius, position, scale, age-scale.x*0.09, dt);
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
    for ( auto& obj : tails ) {

        // TODO> osvietenie napr delfinych chvostov
        //scene.light_positions.at(1) = obj->position + glm::vec3{2, 2, 2};
        auto part = dynamic_cast<FishTail*>(obj.get());
        part->render(scene);
    }
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

void Fish::onClick(Scene &scene) {
    std::cout << "Fish has been clicked!" << std::endl;
}
