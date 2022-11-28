#include "space.h"
#include "scene.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

Cloud::Cloud() {
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("stars.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj");
}

bool Cloud::update(Scene &scene, float dt) {

    // make cloud from polygons

    // TODO urobit oblak z polygonov
    // TODO urobit aby sa oblak pohyboval
    // TODO urobit aby sa oblak pohyboval v zavislosti od pozicie kamery

    // Offset for UV mapping, creates illusion of scrolling
    textureOffset.y -= dt / 5;
    generateModelMatrix();
    return true;
}

void Cloud::render(Scene &scene) {
    // Disable writing to the depth buffer so we render a "background"
    glDepthMask(GL_FALSE);

    // NOTE: this object does not use camera, just renders the entire quad as is
    shader->use();

    // Pass UV mapping offset to the shader
    shader->setUniform("TextureOffset", textureOffset);

    // Render mesh, not using any projections, we just render in 2D
    shader->setUniform("ModelMatrix", modelMatrix);
    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    shader->setUniform("Texture", *texture);
    shader->setUniform("lights.count", 1);
    mesh->render();

    glDepthMask(GL_TRUE);
}

// shared resources
std::unique_ptr<ppgso::Mesh> Cloud::mesh;
std::unique_ptr<ppgso::Shader> Cloud::shader;
std::unique_ptr<ppgso::Texture> Cloud::texture;
