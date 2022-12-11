#ifndef PPGSO_CLOUD_H
#define PPGSO_CLOUD_H

#include <ppgso/ppgso.h>
#include "object.h"

class Cloud final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    static std::unique_ptr<ppgso::Shader> shader;

    Cloud();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_CLOUD_H
