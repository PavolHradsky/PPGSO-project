#include "Ocean.h"
#include "scene.h"
#include "Boat.h"
#include "Dolphin.h"
#include "explosion.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#define SEA_TURBULENCE 0.2f
// shared resources
std::unique_ptr<ppgso::Mesh> Ocean::mesh;
std::unique_ptr<ppgso::Texture> Ocean::texture;
std::unique_ptr<ppgso::Shader> Ocean::shader;

Ocean::Ocean() {
    // Scale the default model
    scale *= 10.0f;
    this->speed = SEA_TURBULENCE;
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("terrain.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Ocean.obj");
}

bool Ocean::update(Scene &scene, float dt) {
    // Fire delay increment
    fireDelay += dt;

    // Hit detection
    for (auto &obj: scene.objects) {
        // Ignore self in scene
        if (obj.get() == this)
            continue;

        // We only need to collide with asteroids, ignore other objects
        auto asteroid = dynamic_cast<Boat *>(obj.get());
        if (!asteroid) continue;

        if (distance(position, asteroid->position) < asteroid->scale.y) {
            // Explode
            auto explosion = std::make_unique<Explosion>();
            explosion->position = position;
            explosion->scale = scale * 3.0f;
            scene.objects.push_back(move(explosion));

            return false;
        }
    }

    // Keyboard controls
//  if(scene.keyboard[GLFW_KEY_LEFT]) {
//    position.x += 10 * dt;
//    rotation.z = -ppgso::PI/4.0f;
//  } else if(scene.keyboard[GLFW_KEY_RIGHT]) {
//    position.x -= 10 * dt;
//    rotation.z = ppgso::PI/4.0f;
//  } else {
//    rotation.z = 0;
//  }
//
//  // Firing projectiles
//  if(scene.keyboard[GLFW_KEY_SPACE] && fireDelay > fireRate) {
//    // Reset fire delay
//    fireDelay = 0;
//    // Invert file offset
//    fireOffset = -fireOffset;
//
//    auto projectile = std::make_unique<Dolphin>();
//    projectile->position = position + glm::vec3(0.0f, 0.0f, 0.3f) + fireOffset;
//    scene.objects.push_back(move(projectile));
//  }

    generateModelMatrix();
    return true;
}

void Ocean::tick() {
    this->position.y += speed;
    this->speed += ((this->position.y <= 0.0f)? 1 : -1 ) * SEA_TURBULENCE;
    if(this->position.y > 0.035f) this->position.y = 0.035f;
    else if(this->position.y < -0.035f) this->position.y = -0.035f;
}

void Ocean::render(Scene &scene) {
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

void Ocean::onClick(Scene &scene) {
    std::cout << "Ocean has been clicked!" << std::endl;
}
