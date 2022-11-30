//
// Created by peter on 26. 11. 2022.
//

#include "Sun.h"
// make rain
#include <glm/gtc/random.hpp>
#include "scene.h"
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/light_frag_glsl.h>
#include <shaders/light_vert_glsl.h>
#include <glm/gtx/euler_angles.hpp>

// Static resources
std::unique_ptr<ppgso::Mesh> Sun::mesh;
std::unique_ptr<ppgso::Texture> Sun::texture;
std::unique_ptr<ppgso::Shader> Sun::shader;

// TODO rain code z copilota, treba sfunkcnit, momentalne tam je 1 gula, ktora by sa dala zneuzit na slnko a z nej spravit luce osvetlenim... alebo nieco ine
Sun::Sun(glm::vec3 position, float minSc, float maxSc, float rndPos) {
    // Set random scale speed and rotation
    this->position = position+10.0f*glm::vec3(0.0f, 1.0f, 0.0f);
    auto rndmPos = random_vec3(-rndPos, rndPos);
    this->position.x += rndmPos.x;
    this->position.y += rndmPos.y;
    this->position.z += rndmPos.z;

    rotation = random_vec3(-0.1, 0.1);
    scale = sameRandom_vec3(minSc, maxSc);

    color = {1,0,0};

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(light_vert_glsl, light_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
}

glm::vec3 Sun::random_vec3 (float mini, float maxi) {
    return {((float) rand() / (float) RAND_MAX) * (maxi - mini) + mini,((float) rand() / (float) RAND_MAX) * (maxi - mini) + mini,  ((float) rand() / (float) RAND_MAX) * (maxi - mini) + mini};
}

glm::vec3 Sun::sameRandom_vec3 (float mini, float maxi) {
    float sur = ((float) rand() / (float) RAND_MAX) * (maxi - mini) + mini;
    return { sur, sur, sur};
}

bool Sun::update(Scene &scene, float dt) {

    posY = position.y;

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();
    //modelMatrix = translateMatrix * glm::eulerAngleYXZ(rotation.x, rotation.y, rotation.z * glm::scale(glm::mat4{1.0f}, scale));
    return true;
}

void Sun::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("light.constant",  1.0f);
    shader->setUniform("light.linear",    0.09f);
    shader->setUniform("light.quadratic", 0.032f);
    shader->setUniform("Material.Shininess", 32.0f);
    shader->setUniform("light.position", posY);
    GLfloat light1_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
    GLfloat spot_direction[] = { 0.0f, 10.0f, 0.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.09f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.032f);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Color", color);

    // Render mesh
    mesh->render();
}



