//
// Created by peter on 26. 11. 2022.
//

#ifndef PPGSO_ROCK_H
#define PPGSO_ROCK_H
#include <ppgso/ppgso.h>

#include "object.h"

class Rock final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;
    float speed = 0;

public:
    /*!
     * Create a new player
     */
    Rock();

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
    float prevCamY = 10;
};

#endif //PPGSO_ROCK_H
