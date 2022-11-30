//
// Created by hrads on 23. 11. 2022.
//
#include <ppgso/ppgso.h>

#include "object.h"
#include "FishTail.h"

#ifndef PPGSO_FISH_H
#define PPGSO_FISH_H

/*!
 * Simple object representing the player
 * Reads keyboard status and manipulates its own position
 * On Update checks collisions with Boat objects in the scene
 */
class Fish final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;
    float age = 0;
    float speed = 0.5;
    float radius = 15;
    std::list< std::unique_ptr<Object> > tails;
public:
    std::unique_ptr<FishTail> fishTail;
    float offset = 0.2891;
    static std::unique_ptr<ppgso::Shader> shader;
    /*!
     * Create a new player
     */
    Fish();

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
    void onClick(Scene &scene) override ;
    float direction = 1;
    float posY = 0;
    float posX = 0;
    float posZ = 0;
    float prevCamY = 10;
};

#endif //PPGSO_FISH_H
