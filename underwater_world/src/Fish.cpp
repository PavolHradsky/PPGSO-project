#include "Fish.h"
#include "Scene.h"
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Fish::mesh;
std::unique_ptr<ppgso::Texture> Fish::texture;
std::unique_ptr<ppgso::Shader> Fish::shader;

Fish::Fish() {
    // Set random scale speed and rotation
    scale *= glm::linearRand(1.0f, 3.0f);
    //speed = {glm::linearRand(-2.0f, 2.0f), glm::linearRand(-5.0f, -10.0f), 0.0f};
    rotation = glm::ballRand(ppgso::PI);
    //rotMomentum = glm::ballRand(ppgso::PI);

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("fish.bmp")); // TODO missing bmp
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("models/dolphin.obj");
}

bool Fish::update(Scene &scene, float dt) {
    return false;
}

void Fish::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("LightColor", scene.lightColor);

    // Set up material
    //shaders->setUniform("MaterialAmbient", 0.3f, 0.3f, 0.3f);
    //shaders->setUniform("MaterialDiffuse", 0.6f, 0.6f, 0.6f);
    //shaders->setUniform("MaterialSpecular", 0.8f, 0.8f, 0.8f);
    shader->setUniform("MaterialShininess", 30.0f);

    // Set up model transform
    shader->setUniform("ModelMatrix", modelMatrix);

    // Render mesh
    mesh->render();
}
