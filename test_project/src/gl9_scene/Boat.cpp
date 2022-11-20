#include <glm/gtc/random.hpp>
#include "Boat.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Boat::mesh;
std::unique_ptr<ppgso::Texture> Boat::texture;
std::unique_ptr<ppgso::Shader> Boat::shader;

Boat::Boat() {
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("boat_diffuse.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("boat.obj");
}

bool Boat::update(Scene &scene, float dt) {
    // Move the boat
    position.x += 0.5f * sin(dt);
    //position.y += 0.5f * cos(dt);
    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void Boat::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

void Boat::onClick(Scene &scene) {
    std::cout << "Boat clicked!" << std::endl;
    //explode(scene, position, {10.0f, 10.0f, 10.0f}, 0 );
}

