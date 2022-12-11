#ifndef PPGSO_SUN_H
#define PPGSO_SUN_H

#include <ppgso/ppgso.h>

#include "object.h"

class Sun final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

    float age = 0;

public:
    glm::vec3 color;

    static std::unique_ptr<ppgso::Shader> shader;

    Sun();
    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    float direction = 1;
    float posY = 0;
    float posZ = 0;
    // translate matrix
    glm::mat4 translateMatrix;

    float timeAlive;
    float minScale;
    float maxScale;
    float randomPosition;
    bool night = false;

};
#endif //PPGSO_SUN_H
