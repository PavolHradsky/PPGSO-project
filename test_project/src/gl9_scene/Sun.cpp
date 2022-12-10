#include "Sun.h"
#include "scene.h"
#include <shaders/texture_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Sun::mesh;
std::unique_ptr<ppgso::Texture> Sun::texture;
std::unique_ptr<ppgso::Shader> Sun::shader;

Sun::Sun() {
    scale *= 25;
    position.y = 190;
    position.x = 0;
    position.z = 0;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sun.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
}


bool Sun::update(Scene &scene, float dt) {
    age += dt;

    if ((!night)) {
        if (age > 20.0) {
            texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("asteroid.bmp"));
            if (scene.nightSwitch)
                scene.lights.strengths[2] = 5;
            else
                scene.lights.strengths[2] = 1;
        }
        if (age > 35.0) {
            age = 5.0;
            night = true;
            if (scene.nightSwitch)
                scene.lights.strengths[2] = 1;
            else
                scene.lights.strengths[2] = 5;
        }

    } else if ((night)) {
        if (age < 20.0) {
            texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sun.bmp"));
            if (scene.nightSwitch)
                scene.lights.strengths[2] = 1;
            else
                scene.lights.strengths[2] = 5;
        }
        if (age > 19.5) {
            night = false;
            if (scene.nightSwitch)
                scene.lights.strengths[2] = 5;
            else
                scene.lights.strengths[2] = 1;

        }
    }
    generateModelMatrix();
    return true;
}

void Sun::render(Scene &scene) {
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



