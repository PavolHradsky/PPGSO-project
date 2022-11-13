//
// Created by peter on 21. 9. 2022.
//

#ifndef UNDERWATER_WORLD_WATER_H
#define UNDERWATER_WORLD_WATER_H

#include <ppgso/ppgso.h>

#include "object.h"

class Water final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 offset;
public:
    Water();

    bool update(Scene &scene, float dt) override;


    void render(Scene &scene) override;
};

#endif //UNDERWATER_WORLD_WATER_H
