#include <ppgso/ppgso.h>
#include "object.h"
#ifndef PPGSO_FISHTAIL_H
#define PPGSO_FISHTAIL_H
/*!
 * Simple object representing the player
 * Reads keyboard status and manipulates its own position
 * On Update checks collisions with Boat objects in the scene
 */
class FishTail final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;
    float distanceX = 0.22;
    float distanceY = -0.01;
    float distanceZ = 0.22;
    glm::vec3 offset;
public:
    static std::unique_ptr<ppgso::Shader> shader;
    /*!
     * Create a new player
     */
    FishTail();

    /*!
     * Update player position considering keyboard inputs
     * @param scene Scene to update
     * @param dt Time delta
     * @return true to delete the object
     */
    bool update(Scene &scene, float dt) override;
    void updateTail(Scene &scene, float posX, float posY, float posZ, float speed, float radius, glm::vec3 pos, glm::vec3 sc, float age, float dt);
    /*!
     * Render player
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

    float direction = 1;
    float prevCamY = 10;
};

#endif //PPGSO_FISHTAIL_H
