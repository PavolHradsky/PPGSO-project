#include <glm/gtc/random.hpp>
#include "Boat.h"
#include "Dolphin.h"
#include "explosion.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Boat::mesh;
std::unique_ptr<ppgso::Texture> Boat::texture;
std::unique_ptr<ppgso::Shader> Boat::shader;

Boat::Boat() {
  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("boat_diffuse.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("boat.obj");
}

bool Boat::update(Scene &scene, float dt) {
  // Count time alive
  age += dt;

  // Animate position according to time
  position += speed * dt;

  // Rotate the object
  rotation += rotMomentum * dt;

  // Delete when alive longer than 10s or out of visibility
  if (age > 10.0f || position.y < -10) return false;

  // Collide with scene
  for (auto &obj : scene.objects) {
    // Ignore self in scene
    if (obj.get() == this) continue;

    // We only need to collide with asteroids and projectiles, ignore other objects
    auto asteroid = dynamic_cast<Boat*>(obj.get()); // dynamic_pointer_cast<Boat>(obj);
    auto projectile = dynamic_cast<Dolphin*>(obj.get()); //dynamic_pointer_cast<Dolphin>(obj);
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
      explode(scene, (obj->position + position) / 2.0f, (obj->scale + scale) / 2.0f, pieces);

      // Destroy self
      return false;
    }
  }

  // Generate modelMatrix from position, rotation and scale
  generateModelMatrix();

  return true;
}

void Boat::explode(Scene &scene, glm::vec3 explosionPosition, glm::vec3 explosionScale, int pieces) {
  // Generate explosion
  auto explosion = std::make_unique<Explosion>();
  explosion->position = explosionPosition;
  explosion->scale = explosionScale;
  explosion->speed = speed / 2.0f;
  scene.objects.push_back(move(explosion));
}

void Boat::render(Scene &scene) {
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

void Boat::onClick(Scene &scene) {
  std::cout << "Boat clicked!" << std::endl;
  explode(scene, position, {10.0f, 10.0f, 10.0f}, 0 );
  age = 10000;
}

