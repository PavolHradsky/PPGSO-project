#ifndef PPGSO_SUN_H
#define PPGSO_SUN_H

#include <ppgso/ppgso.h>

#include "object.h"

class Sun final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

    float age = 0;

public:
    glm::vec3 color;

    static std::unique_ptr<ppgso::Shader> shader;
    /*!
     * Create a sun
     */
    Sun();
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
    bool night = false;

};
#endif //PPGSO_SUN_H
