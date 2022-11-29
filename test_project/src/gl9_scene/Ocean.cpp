#include "Ocean.h"
#include "scene.h"
#include "Boat.h"
#include "Dolphin.h"
#include "explosion.h"
#include "PerlinNoise.h"
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#define SEA_TURBULENCE 0.00f
// shared resources
//std::unique_ptr<ppgso::Mesh> Ocean::mesh;
std::unique_ptr<ppgso::Texture> Ocean::texture;
std::unique_ptr<ppgso::Shader> Ocean::shader;



Ocean::Ocean() {
    // Scale the default model
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Ocean.bmp"));
}


bool Ocean::update(Scene &scene, float dt) {

    age += dt;

    PerlinNoise pn(0.1f, 0.1f, 0.1f, 0.1f);
    std::cout << "Perlin noise: " << bezier.vertices.size() << std::endl;
    for (int i = 0; i < bezier.vertices.size(); i++) {
        bezier.vertices[i].y = pn.noise(bezier.vertices[i].x, bezier.vertices[i].z, 0.8f);
    }

//    controlPoints[0][0][1] = std::sin(age);
//
//    bezier.bezierPatch(controlPoints);

    bezier.update();
    bezier.render(scene);

    return true;
}

void Ocean::render(Scene &scene) {

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

void Ocean::onClick(Scene &scene) {
    std::cout << "Ocean has been clicked!" << std::endl;
}