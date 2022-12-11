#include <ppgso/ppgso.h>
#include "object.h"
#ifndef PPGSO_FISHTAIL_H
#define PPGSO_FISHTAIL_H

class FishTail final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    static std::unique_ptr<ppgso::Shader> shader;

    FishTail();
    bool update(Scene &scene, float dt) override;
    void updateTail(Scene &scene, float posX, float posY, float posZ, float speed, float radius, glm::vec3 pos, glm::vec3 sc, float age, float dt);

    void render(Scene &scene) override;

};

#endif //PPGSO_FISHTAIL_H
