#ifndef PPGSO_STAR_H
#define PPGSO_STAR_H


#include <ppgso/ppgso.h>

#include "object.h"

class Star final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

    float age = 0;

public:

    static std::unique_ptr<ppgso::Shader> shader;

    explicit Star(glm::vec3 pos);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;


    float direction = 1;
    float posY = 0;
    float posZ = 0;
    // translate matrix

};

#endif //PPGSO_STAR_H
