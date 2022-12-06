#include "scene.h"
#include "Ocean.h"
#include "UnderWatterTerrain.h"
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include "shaders/my_texture_frag_glsl.h"
#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>
#include "shaders/my_phong_frag_glsl.h"

// shared resources
std::unique_ptr<ppgso::Mesh> UnderWatterTerrain::mesh;
std::unique_ptr<ppgso::Shader> UnderWatterTerrain::shader;
std::unique_ptr<ppgso::Texture> UnderWatterTerrain::texture;

UnderWatterTerrain::UnderWatterTerrain() {

    scale = {1.2f, 0.4, 1.2f};

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("wall.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("underWater4.obj");
}

bool UnderWatterTerrain::update(Scene &scene, float dt) {

    if(prevCamY > 0 && scene.camera->position.y < 0){
        shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, my_phong_frag_glsl);
    }
    if(prevCamY < 0 && scene.camera->position.y > 0){
        shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    }
    prevCamY = scene.camera->position.y;

    generateModelMatrix();
    return true;
}

void UnderWatterTerrain::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("CamPos", scene.camera->position);
    shader->setUniform("global_lighting_on", scene.global_lighting_on);

//    shader->setUniform("material.ambient", {1, 1, 1});
    shader->setUniform("material.diffuse", {1, 1, 1});
    shader->setUniform("material.specular", {0.9f, 0.9f, 0.9f});
    shader->setUniform("material.shininess", 32.0f);

    for (int i = 0; i < scene.lights.count; i++) {
        shader->setUniform("lights.positions[" + std::to_string(i) + "]", scene.lights.positions[i]);
        shader->setUniform("lights.colors[" + std::to_string(i) + "]", scene.lights.colors[i]);
        shader->setUniform("lights.ranges[" + std::to_string(i) + "]", scene.lights.ranges[i]);
        if (scene.lights.strengths[i] < 0) {
            shader->setUniform("lights.strengths[" + std::to_string(i) + "]", 0.0f);
        }
        else {
            shader->setUniform("lights.strengths[" + std::to_string(i) + "]", scene.lights.strengths[i]);
        }
    }

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    shader->setUniform("UseShadow", false);
    mesh->render();
}

