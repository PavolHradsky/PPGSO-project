#ifndef PPGSO_FILTER_H
#define PPGSO_FILTER_H

#include <ppgso/ppgso.h>
#include "object.h"

/*!
 * Simple object representing the player
 * Reads keyboard status and manipulates its own position
 * On Update checks collisions with Boat objects in the scene
 */
class Filter final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;
    float speed = 0;
public:
    static std::unique_ptr<ppgso::Shader> shader;

    /*!
     * Create a new player
     */
    Filter();

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
    float prevCamY = 10;

};


#endif //PPGSO_FILTER_H
