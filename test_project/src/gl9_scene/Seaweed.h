#ifndef PPGSO_SEAWEED_H
#define PPGSO_SEAWEED_H
#include <ppgso/ppgso.h>
#include "object.h"

class Seaweed final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    static std::unique_ptr<ppgso::Shader> shader;

    Seaweed();


    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
    float posY = 0;

};

#endif //PPGSO_SEAWEED_H
