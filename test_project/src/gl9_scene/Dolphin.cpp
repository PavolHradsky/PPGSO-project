#include <glm/gtc/random.hpp>
#include "scene.h"
#include "Dolphin.h"
#include "explosion.h"
#include "Boat.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Dolphin::mesh;
std::unique_ptr<ppgso::Shader> Dolphin::shader;
std::unique_ptr<ppgso::Texture> Dolphin::texture;

Dolphin::Dolphin() {
    scale *= glm::linearRand(0.009f, 0.015f);
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("dolphin.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("dolphin.obj");
}

bool Dolphin::update(Scene &scene, float dt) {
    age += dt;
    position.z += speed * dt * direction;
    if(position.z > 40){
        direction = -1;
        rotation.y = ppgso::PI;
    }
    if(position.z < -40){
        direction = 1;
        rotation.y = 0;
    }
    if(direction == 1){
        position.y = std::cos(age * speed) * radius + posY;
//        position.z = std::sin(age * speed) * radius + posZ;

//        rotation.x += dt*speed;
        rotation.x = std::sin(age * speed) * radius + 3*ppgso::PI/2;
    }else{
        position.y = std::sin(age * speed) * radius + posY;
//        position.z = -std::cos(age * speed) * radius + posZ;

//        rotation.x -= dt*speed;
        rotation.x = std::cos(age * speed) * radius + 3*ppgso::PI/2;
    }
    // make collision of dolhpin and boat
    for (auto &object : scene.objects) {
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
            // Create explosion
            std::cout << "Explosion" << std::endl;
            scene.objects.push_back(std::make_unique<Explosion>(boat->position));

            if (direction == 1) {
                direction = -1;
            } else {
                direction = 1;
            }
            this->rotation.y = ppgso::PI;
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

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}
