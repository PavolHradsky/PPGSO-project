#include "scene.h"
#include "Ocean.h"
#include "UnderWatterTerrain.h"
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include "shaders/my_texture_frag_glsl.h"

// shared resources
std::unique_ptr<ppgso::Mesh> UnderWatterTerrain::mesh;
std::unique_ptr<ppgso::Shader> UnderWatterTerrain::shader;
std::unique_ptr<ppgso::Texture> UnderWatterTerrain::texture;

UnderWatterTerrain::UnderWatterTerrain() {

    scale = {1.2f, 0.4, 1.2f};

    // TODO Chyba tu este vrchna cast "kocky", hodilo by sa nejako vyplnit vnutro object modrym, nech mame pocit ze sme vo vode
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("wall.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("underWater4.obj");
}

bool UnderWatterTerrain::update(Scene &scene, float dt) {

//    if(prevCamY > 0 && scene.camera->position.y < 0){
//        shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, my_texture_frag_glsl);
//    }
//    if(prevCamY < 0 && scene.camera->position.y > 0){
//        shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
//    }
//    prevCamY = scene.camera->position.y;

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

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

