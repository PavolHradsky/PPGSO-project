#include <glm/gtc/random.hpp>
#include "scene.h"
#include "Dolphin.h"

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Dolphin::mesh;
std::unique_ptr<ppgso::Shader> Dolphin::shader;
std::unique_ptr<ppgso::Texture> Dolphin::texture;

Dolphin::Dolphin(glm::vec3 pos, glm::vec3 rot, float freq) {
  position = pos;
  rotation = rot;
  frequency = freq;

  if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("flag.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("dolphin.obj");
}

bool Dolphin::update(Scene &scene, float dt) {
  // Accelerate
  //speed += glm::vec3{0.0f, 20.0f, 0.0f} * dt;
  position.x = -(cos(dt)) / 100;
  position.y = (sin(dt)) / 100;
  //rotation += speed;
  rotation_ += frequency;
  // Move the projectile
  rotation.x = rotation_;

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
