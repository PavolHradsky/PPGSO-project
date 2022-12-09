//
// Created by hrads on 9. 12. 2022.
//

#ifndef PPGSO_SHADOW_H
#define PPGSO_SHADOW_H

#include <ppgso/ppgso.h>
#include "Scene.h"
#include "object.h"

class Shadow final : public Object {
private:
    float time;
    glm::mat4 translateMatrix;
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;
    glm::mat4 offset;

public:
    glm::vec3 color;
    static std::unique_ptr<ppgso::Shader> shader;
    float speed = 0;
    Shadow();
    bool update(Scene &scene, float dt) override;
    void render(Scene &scene) override;
};

#endif //PPGSO_SHADOW_H
