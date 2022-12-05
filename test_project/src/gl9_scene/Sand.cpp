#include "Sand.h"
#include "scene.h"
#include "Boat.h"
#include "Dolphin.h"
#include "explosion.h"
#include "PerlinNoise.h"
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include <shaders/my_texture_frag_glsl.h>
#define SEA_TURBULENCE 0.00f
// shared resources
//std::unique_ptr<ppgso::Mesh> Sand::mesh;
std::unique_ptr<ppgso::Texture> Sand::texture;
std::unique_ptr<ppgso::Shader> Sand::shader;



Sand::Sand() {
    // Scale the default model
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, my_texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Sand2.bmp"));

}


bool Sand::update(Scene &scene, float dt) {

    bezier.update();
    bezier.render(scene);

    return true;
}

void Sand::render(Scene &scene) {

    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    //mesh->render();
}

void Sand::onClick(Scene &scene) {
    std::cout << "Ocean has been clicked!" << std::endl;
}