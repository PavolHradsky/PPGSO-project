//
// Created by peter on 3. 12. 2022.
//

#ifndef PPGSO_SEAWEED_H
#define PPGSO_SEAWEED_H
#include <ppgso/ppgso.h>

#include "object.h"
class Seaweed final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    static std::unique_ptr<ppgso::Shader> shader;
    /*!
     * Create a new player
     */
    Seaweed();

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
    float posY = 0;
    float posX = 0;
    float posZ = 0;

};

#endif //PPGSO_SEAWEED_H
