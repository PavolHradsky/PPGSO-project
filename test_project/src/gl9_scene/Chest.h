//
// Created by hrads on 7. 12. 2022.
//

#ifndef PPGSO_CHEST_H
#define PPGSO_CHEST_H

#include <ppgso/ppgso.h>

#include "object.h"

class Chest final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    static std::unique_ptr<ppgso::Shader> shader;

    Chest();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_CHEST_H
