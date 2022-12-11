#ifndef PPGSO_SHADOW_H
#define PPGSO_SHADOW_H

#include <ppgso/ppgso.h>
#include "Scene.h"
#include "object.h"

class Shadow final : public Object {
private:
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    glm::vec3 color;
    static std::unique_ptr<ppgso::Shader> shader;

    Shadow();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_SHADOW_H
