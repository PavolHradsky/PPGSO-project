#ifndef UNDERWATER_WORLD_FISH_H
#define UNDERWATER_WORLD_FISH_H

#include <ppgso/ppgso.h>
#include "Object.h"

class Fish : public Object {
private:
    // Static resources
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;
    static std::unique_ptr<ppgso::Shader> shader;
public:
    Fish();
    bool update(Scene &scene, float dt) override;
    void render(Scene &scene) override;

};
#endif //UNDERWATER_WORLD_FISH_H