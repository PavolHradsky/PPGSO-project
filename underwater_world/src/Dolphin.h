//
// Created by peter on 31. 10. 2022.
//

#ifndef UNDERWATER_WORLD_DOLPHIN_H
#define UNDERWATER_WORLD_DOLPHIN_H

#include <ppgso/ppgso.h>

#include "Scene.h"
#include "Object.h"

class Dolphin final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 offset;
public:
    Dolphin();

    bool update(Scene &scene, float dt) override;


    void render(Scene &scene) override;
};

#endif //UNDERWATER_WORLD_DOLPHIN_H
