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

    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;
    glm::mat4 offset;
    float rotationZ = -0.01;
    float rotSpeed = .08f;

    float distanceX = 0.85;
    float distanceY = 0;
    float distanceZ = 0;


public:
    Bubble(glm::vec3  position, float timeAlive, float minSc, float maxSc, float rndPos);

    Bubble(glm::mat4 translateMatrix, float timeAlive, float minSc, float maxSc, float rndPos);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

private:
    glm::vec3 sameRandom_vec3 (float mini, float maxi);
    glm::vec3 random_vec3 (float mini, float maxi);
};

#endif //PPGSO_BUBBLE_H
