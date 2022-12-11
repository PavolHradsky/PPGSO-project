//
// Created by peter on 26. 11. 2022.
//

#ifndef PPGSO_BUBBLE_H
#define PPGSO_BUBBLE_H

#include <ppgso/ppgso.h>
#include "Scene.h"
#include "object.h"
#include "Sun.h"

class Bubble final : public Object {
private:
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    glm::vec3 color;
    static std::unique_ptr<ppgso::Shader> shader;
    float speed = 0;

    Bubble();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_BUBBLE_H
