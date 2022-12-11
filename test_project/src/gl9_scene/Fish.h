#include <ppgso/ppgso.h>
#include "object.h"
#include "FishTail.h"

#ifndef PPGSO_FISH_H
#define PPGSO_FISH_H


class Fish final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;
    float age = 0;
    float speed = 0.2;
    float radius = 15;
    std::list< std::unique_ptr<Object> > tails;

public:
    static std::unique_ptr<ppgso::Shader> shader;

    Fish();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    float posY = 0;
    float posX = 0;
    float posZ = 0;
    bool animate = true;
    bool my_fish = false;
};

#endif //PPGSO_FISH_H
