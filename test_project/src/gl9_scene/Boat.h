#pragma once

#include <memory>
#include <ppgso/ppgso.h>
#include "scene.h"
#include "object.h"

class Boat final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

    // Speed and rotational momentum
    float speed = 10;
    float age{0.0f};
public:
    bool animate;
    bool generator = false;
    static std::unique_ptr<ppgso::Shader> shader;

    /*!
     * Create new asteroid
     */
    Boat();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

