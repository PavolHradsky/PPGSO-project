#ifndef PPGSO_LIGHTHOUSE_H
#define PPGSO_LIGHTHOUSE_H
#pragma once
#include <ppgso/ppgso.h>
#include "object.h"

class LightHouse final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    static std::unique_ptr<ppgso::Shader> shader;

    LightHouse();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
    float age = 0;
};

#endif //PPGSO_LIGHTHOUSE_H
