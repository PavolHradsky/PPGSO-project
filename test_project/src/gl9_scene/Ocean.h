#pragma once

#include <ppgso/ppgso.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include "object.h"
#include "scene.h"
#include "surface.h"

class Ocean final : public Object {
private:
  // Static resources (Shared between instances)
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Texture> texture;
  float speed = 0;

public:
    static std::unique_ptr<ppgso::Shader> shader;
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
    void onClick(Scene &scene) override;

    glm::vec3 globalPosition = {0, 0, 0};
    float age = 0;
};

