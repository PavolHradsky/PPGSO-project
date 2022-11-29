#pragma once

#include <ppgso/ppgso.h>
#include "object.h"

class Dolphin final : public Object {
private:
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

    float age = 0;

public:
    static std::unique_ptr<ppgso::Shader> shader;
    Dolphin();

    bool update(Scene &scene, float dt) override;


    void render(Scene &scene) override;

    float speed = 4;
    float radius = 2;
    float posX = 0;
    float posY = 0;
    float posZ = 0;
    float direction = 1;
    float explosionTime = 0;
    float prevCamY = 10;
};

