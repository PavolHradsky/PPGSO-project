#ifndef PPGSO_FILTER_H
#define PPGSO_FILTER_H

#include <ppgso/ppgso.h>
#include "object.h"

class Filter final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    static std::unique_ptr<ppgso::Shader> shader;

    Filter();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};


#endif //PPGSO_FILTER_H
