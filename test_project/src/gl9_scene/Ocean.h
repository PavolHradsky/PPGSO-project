#pragma once
#include <ppgso/ppgso.h>

#include "object.h"

/*!
 * Simple object representing the player
 * Reads keyboard status and manipulates its own position
 * On Update checks collisions with Boat objects in the scene
 */
class Ocean final : public Object {
private:
  // Static resources (Shared between instances)
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Shader> shader;
  static std::unique_ptr<ppgso::Texture> texture;
  float speed = 0;

  // Delay fire and fire rate
  float fireDelay{0.0f};
  float fireRate{0.1f};
  glm::vec3 fireOffset{0.7f,0.0f,0.0f};

public:
  /*!
   * Create a new player
   */
  Ocean();

  /*!
   * Update player position considering keyboard inputs
   * @param scene Scene to update
   * @param dt Time delta
   * @return true to delete the object
   */
  bool update(Scene &scene, float dt) override;

  /*!
   * Render player
   * @param scene Scene to render in
   */
  void render(Scene &scene) override;


  /*!
   * Ocean click event
   * @param scene
   */
  void onClick(Scene &scene) override ;
};

