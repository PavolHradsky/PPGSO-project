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

Dolphin::Dolphin(glm::vec3 pos, glm::vec3 rot, float freq) {
    position = pos;
    rotation = rot;
    frequency = freq;
    scale *= glm::linearRand(0.0009f, 0.0015f);
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("dolphin.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("dolphin.obj");
}

bool Dolphin::update(Scene &scene, float dt) {
    age += dt;
    position.x = position.x;
    position.y = position.y ;

    rotation_ += frequency;
    rotation.x = rotation_;
    // Rotate the object
    // Delete when alive longer than 10s
    //if (age > 10.0f ) return false;

    // TODO make collision dolphin with boat and dolphin will be drowned
    for (auto &object : scene.objects) {
        // We only want to collide with other boats
        auto dolphin = dynamic_cast<Dolphin*>(object.get());
        if (!dolphin) continue;
        // Check distance between objects
        // obtain position of the boat
        auto boat = std::make_unique<Boat>();
        //if (!boat) continue;
        // Check distance between objects
        auto distance = glm::distance(boat->position, dolphin->position);
        //std::cout << "Distance: " << distance << std::endl;

        // If the distance is smaller than sum of their scales, we have a collision
        if (distance < scale.x + dolphin->scale.x) {
            // Create explosion
            auto explosion = std::make_unique<Explosion>();
            explosion->position = dolphin->position;
            scene.objects.push_back(std::move(explosion));
            // Delete this object
            return false;
        }
    }
    /*
  // Collide with scene
  for (auto &obj : scene.objects) {
    // Ignore self in scene
    if (obj.get() == this) continue;

    // We only need to collide with asteroids and projectiles, ignore other objects
    auto projectile = dynamic_cast<Boat*>(obj.get()); // dynamic_pointer_cast<Boat>(obj);
    auto asteroid = dynamic_cast<Dolphin*>(obj.get()); //dynamic_pointer_cast<Dolphin>(obj);
    if (!asteroid && !projectile) continue;

    // When colliding with other asteroids make sure the object is older than .5s
    // This prevents excessive collisions when asteroids explode.
    if (asteroid && age < 0.5f) continue;

    // Compare distance to approximate size of the asteroid estimated from scale.
    if (distance(position, obj->position) < (obj->scale.y + scale.y) * 0.7f) {
      int pieces = 3;

      // Too small to split into pieces
      if (scale.y < 0.5) pieces = 0;

      // Generate smaller asteroids
      //explode(scene, (obj->position + position) / 2.0f, (obj->scale + scale) / 2.0f, pieces);

      // Destroy self
      return false;
    }
  }*/
    // Accelerate
    //speed += glm::vec3{0.0f, 20.0f, 0.0f} * dt;
/*
    position.x = position.x;
    position.y = position.y ;
    position +=position;
    rotation_ += frequency;
    // Move the projectile
    rotation.x = rotation_;
*/
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

    shader->setUniform("material.ambient", {0.05f, 0.05f, 0.05f});
    shader->setUniform("material.diffuse", {0.8f, 0.8f, 0.8f});
    shader->setUniform("material.specular", {0.9f, 0.9f, 0.9f});
    shader->setUniform("material.shininess", 32.0f);
    for (int i = 0; i < 1; i++) {
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
    mesh->render();
}
