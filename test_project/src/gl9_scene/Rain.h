//
// Created by hrads on 5. 12. 2022.
//

#ifndef PPGSO_RAIN_H
#define PPGSO_RAIN_H

#include <ppgso/ppgso.h>

#include "object.h"

/*!
 * Simple object representing the player
 * Reads keyboard status and manipulates its own position
 * On Update checks collisions with Boat objects in the scene
 */
class Rain final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

    // TODO: add more parameters as needed
    glm::vec3 speed;
    glm::vec3 color;
    glm::vec3 scale;

    float mass;

    Scene *scene;

    float time = 0;
    float lifetime = 10;
public:
    glm::vec3 position;
    bool isGravityAffected;

    /// Construct a new Particle
    /// \param p - Initial position
    /// \param s - Initial speed
    /// \param c - Color of particle
    Rain(glm::vec3 p, glm::vec3 s, glm::vec3 c, glm::vec3 scale, Scene *scene, bool isGravityAffected, float mass);

    void calculateGravitySpeed();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    static std::unique_ptr<ppgso::Shader> shader;
};


#endif //PPGSO_RAIN_H
