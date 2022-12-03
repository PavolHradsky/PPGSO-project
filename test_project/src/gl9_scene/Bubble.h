//
// Created by peter on 26. 11. 2022.
//

#ifndef PPGSO_BUBBLE_H
#define PPGSO_BUBBLE_H
#include <ppgso/ppgso.h>
#include "Scene.h"
#include "object.h"

class Bubble final : public Object {
private:
    float time;
    glm::vec3 color;
    glm::mat4 translateMatrix;
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;
    glm::mat4 offset;


public:
    static std::unique_ptr<ppgso::Shader> shader;
    Bubble(glm::mat4 translateMatrix, float timeAlive, float rndPos);
    float minScale = 0.05, maxScale = 0.1;
    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

private:
    static glm::vec3 random_vec3 (float mini, float maxi);
};

#endif //PPGSO_BUBBLE_H
