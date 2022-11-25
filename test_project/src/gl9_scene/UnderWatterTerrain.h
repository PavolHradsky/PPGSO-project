//
// Created by peter on 25. 11. 2022.
//
#pragma once

#include <ppgso/ppgso.h>
#include "object.h"

#ifndef PPGSO_UNDERWATTERTERRAIN_H
#define PPGSO_UNDERWATTERTERRAIN_H

class UnderWatterTerrain final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;
    float speed = 0;

public:
    /*!
     * Create a new player
     */
    UnderWatterTerrain();

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


};

#endif //PPGSO_UNDERWATTERTERRAIN_H
