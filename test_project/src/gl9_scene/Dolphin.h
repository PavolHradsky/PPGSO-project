#pragma once
#include <ppgso/ppgso.h>
#include "object.h"

class Dolphin final : public Object {
private:
  static std::unique_ptr<ppgso::Shader> shader;
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Texture> texture;

  glm::vec3 speed;
  glm::vec3 rotMomentum;
  float frequency;
  float rotation_;
public:
  /*
   * Create new projectile
   */
  Dolphin(glm::vec3 pos, glm::vec3 rot, float freq);

  /*!
   * Update projectile position
   * @param scene Scene to update
   * @param dt Time delta
   * @return true to delete the object
   */
  bool update(Scene &scene, float dt) override;

  /*!
   * Render projectile
   * @param scene Scene to render in
   */
  void render(Scene &scene) override;

};

