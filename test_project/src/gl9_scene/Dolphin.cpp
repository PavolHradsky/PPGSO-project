#include <glm/gtc/random.hpp>
#include "scene.h"
#include "Dolphin.h"
#include "Boat.h"
#include "LightHouse.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include "Star.h"

// shared resources
std::unique_ptr<ppgso::Mesh> Dolphin::mesh;
std::unique_ptr<ppgso::Shader> Dolphin::shader;
std::unique_ptr<ppgso::Texture> Dolphin::texture;

Dolphin::Dolphin() {
    scale *= glm::linearRand(0.009f, 0.015f);
    rotation.x = 5 * ppgso::PI / 3;
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("dolphin.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("dolphin.obj");
}

bool Dolphin::update(Scene &scene, float dt) {

    age += dt;
    cooldown += dt;

    position.z += speed * dt * direction;
    if (position.z > 120) {
        direction = -1;
        rotation.y = ppgso::PI;
    }
    if (position.z < -120) {
        direction = 1;
        rotation.y = 0;
    }
    if (direction == 1) {
        position.y = std::cos(age * speed) * radius + posY;
        rotation.x = std::sin(age * speed) * radius + 5 * ppgso::PI / 3;
        +posX;
    } else {
        position.y = std::sin(age * speed) * radius + posY;
        rotation.x = std::cos(age * speed) * radius + 5 * ppgso::PI / 3 + posX;
    }
    //make collision between 2 dolphins

    if (cooldown > 2) {

        for (auto &object: scene.objects) {
            // Ignore self in scene
            if (object.get() == this)
                continue;
            // We only need to collide with boats
            auto dolphin = dynamic_cast<Dolphin *>(object.get());
            if (!dolphin)
                continue;

            // Check distance between objects

            auto distance = glm::distance(this->position, dolphin->position);
            if (distance < 1) {
                cooldown = 0;
                for (int j = 0; j < 10; j++) {
                    scene.objects.push_back(std::make_unique<Star>(position));
                }

                while (this->position.y > -80 || dolphin->position.y > -80) {
                    //make dolphin falling down
                    this->position.y -= 1;
                    this->position.x = this->position.x;
                    this->position.z = this->position.z;
                    dolphin->position.y -= 1;
                    dolphin->position.x = dolphin->position.x;
                    dolphin->position.z = dolphin->position.z;
                }
            }

        }
        // make collision of dolhpin and boat
        for (auto &object: scene.objects) {
            // Ignore self in scene
            if (object.get() == this)
                continue;
            // We only need to collide with boats
            auto boat = dynamic_cast<Boat *>(object.get());
            if (!boat)
                continue;

            // Check distance between objects

            auto distance = glm::distance(this->position, boat->position);
            if (distance < 5) {
                cooldown = 0;
                for (int j = 0; j < 10; j++) {
                    scene.objects.push_back(std::make_unique<Star>(position));
                }
                if (direction == 1) {
                    rotation.y = ppgso::PI;
                }
                if (direction == -1) {
                    rotation.y = 0;
                }
                direction *= -1;
            }

        }

        for (auto &object: scene.objects) {
            // Ignore self in scene
            if (object.get() == this)
                continue;
            // TODO make collision with lightHouse
            auto lighthouse = dynamic_cast<LightHouse *>(object.get());
            if (!lighthouse)
                continue;

            // Check distance between objects
            auto distance = glm::distance(this->position, lighthouse->position);
            if (distance < 1) {
                cooldown = 0;
                for (int j = 0; j < 10; j++) {
                    scene.objects.push_back(std::make_unique<Star>(position));
                }

                std::cout << "Collision dolphin lighthouse" << std::endl;

                if (direction == 1) {
                    rotation.y = ppgso::PI;
                }
                if (direction == -1) {
                    rotation.y = 0;
                }
                direction *= -1;
            }

        }
    }

    generateModelMatrix();
    return true;
}


void Dolphin::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("global_lighting_on", scene.global_lighting_on);

    shader->setUniform("material.ambient", {0.5f, 0.5f, 0.5f});
    shader->setUniform("material.diffuse", {0.8f, 0.8f, 0.8f});
    shader->setUniform("material.specular", {0.9f, 0.9f, 0.9f});
    shader->setUniform("material.shininess", 100.0f);

    shader->setUniform("lights.count", 5);
    for (int i = 0; i < 5; i++) {
        shader->setUniform("lights.positions[" + std::to_string(i) + "]", scene.lights.positions[i]);
        shader->setUniform("lights.colors[" + std::to_string(i) + "]", scene.lights.colors[i]);
        shader->setUniform("lights.ranges[" + std::to_string(i) + "]", scene.lights.ranges[i]);
        if (scene.lights.strengths[i] < 0) {
            shader->setUniform("lights.strengths[" + std::to_string(i) + "]", 0.9f);
        } else {
            shader->setUniform("lights.strengths[" + std::to_string(i) + "]", scene.lights.strengths[i]);
        }
    }
    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}