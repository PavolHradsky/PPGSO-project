//
// Created by peter on 26. 11. 2022.
//

#ifndef PPGSO_RAIN_H
#define PPGSO_RAIN_H

#include <ppgso/ppgso.h>

#include "object.h"
// create rain

class Rain final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

    float age = 0;

public:
    static std::unique_ptr<ppgso::Shader> shader;
    /*!
     * Create a new player
     */
    Rain(glm::vec3 position,  float minSc, float maxSc, float rndPos);
    glm::vec3 sameRandom_vec3 (float mini, float maxi);
    glm::vec3 random_vec3 (float mini, float maxi);
    // yellow color
    glm::vec3 color = {1,0,0};
    /*!
     * Update player position considering keyboard inputs
     * @param scene Scene to update
     * @param dt Time delta
     * @return true to delete the object
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render player
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

    /*!
     * Ocean click event
     * @param scene
     */
    float direction = 1;
    float posY = 0;
    float posZ = 0;
    // translate matrix
    glm::mat4 translateMatrix;

    float timeAlive;
    float minScale;
    float maxScale;
    float randomPosition;

};
#endif //PPGSO_RAIN_H
