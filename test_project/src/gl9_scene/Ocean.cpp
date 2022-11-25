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
std::unique_ptr<ppgso::Mesh> Ocean::mesh;
std::unique_ptr<ppgso::Texture> Ocean::texture;
std::unique_ptr<ppgso::Shader> Ocean::shader;

Ocean::Ocean() {
    // Scale the default model
    scale *= 20.0f;
    this->speed = SEA_TURBULENCE;
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Ocean.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Ocean.obj");
    // apply perlin noise on ocean object
    // TODO, also implemented in mesh.h and mesh.cpp
    PerlinNoise pn(0.1f, 0.1f, 0.1f, 0.1f);
    for (int i = 0; i < mesh->vertices.size(); i++) {
        mesh->vertices[i].position.z += pn.noise(mesh->vertices[i].position.x, mesh->vertices[i].position.y, 0.0f);
    }
    /*
    double value[100][100];
    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < 100; x++) {
            float nx = x/100 - 0.5,
                    ny = y/100 - 0.5;
            value[y][x] = PerlinNoise::noise(nx, ny);
        }
    }*/
}


bool Ocean::update(Scene &scene, float dt) {
    this->position.y += speed;
    this->position.x += speed;
    this->speed += ((this->position.y <= 0.0f)? 1 : -1 ) * SEA_TURBULENCE;
    if(this->position.y > 0.035f) this->position.y = 0.035f;
    else if(this->position.y < -0.035f) this->position.y = -0.035f;

    generateModelMatrix();
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
    mesh->render();
}

void Ocean::onClick(Scene &scene) {
    std::cout << "Ocean has been clicked!" << std::endl;
}
