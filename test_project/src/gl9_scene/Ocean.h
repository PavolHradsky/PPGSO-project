#pragma once

#include <ppgso/ppgso.h>
#include "object.h"

class Ocean final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;
    float speed = 0;

public:
    static std::unique_ptr<ppgso::Shader> shader;

    Ocean();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    glm::vec3 globalPosition = {0, 0, 0};

    float prevCamY = 10;
};

