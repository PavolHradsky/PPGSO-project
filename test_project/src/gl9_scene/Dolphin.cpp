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

    position.y = std::cos(age * speed) * radius + posY;
    position.z = std::sin(age * speed) * radius + posZ;

    rotation.x += dt*speed;

//    // TODO make collision dolphin with boat and dolphin will be drowned
//    for (auto &object : scene.objects) {
//        // We only want to collide with other boats
//        auto dolphin = dynamic_cast<Dolphin*>(object.get());
//        if (!dolphin) continue;
//        // Check distance between objects
//        // obtain position of the boat
//        auto boat = std::make_unique<Boat>();
//        //if (!boat) continue;
//        // Check distance between objects
//        auto distance = glm::distance(boat->position, dolphin->position);
//        //std::cout << "Distance: " << distance << std::endl;
//
//        // If the distance is smaller than sum of their scales, we have a collision
//        if (distance < scale.x + dolphin->scale.x) {
//            // Create explosion
//            auto explosion = std::make_unique<Explosion>();
//            explosion->position = dolphin->position;
//            scene.objects.push_back(std::move(explosion));
//            // Delete this object
//            return false;
//        }
//    }

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
