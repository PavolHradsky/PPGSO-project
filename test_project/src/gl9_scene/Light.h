//
// Created by peter on 6. 12. 2022.
//

#ifndef PPGSO_LIGHT_H
#define PPGSO_LIGHT_H
#include <ppgso/ppgso.h>

#include "object.h"


class Light final : public Object {
private:

    glm::vec3 lightPos = {50,32.8*0.05f, 50};
    glm::vec3 lightColor = {1.0f, 1.0f, 1.0f};

public:
    static std::unique_ptr<ppgso::Shader> shader_light;
    static std::unique_ptr<ppgso::Shader> shader_cube;
    std::vector<float> vertices;
    unsigned int VBO, cubeVAO;
    unsigned int lightCubeVAO;
    /*!
     * Create a new player
     */
    Light();

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

#endif //PPGSO_LIGHT_H
