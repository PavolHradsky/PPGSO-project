#include <cmath>
#include <glm/gtc/random.hpp>
#include "Boat.h"
#include "Bubble.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include "shaders/texture_vert_glsl.h"
#include "shaders/texture_frag_glsl.h"
#include "shaders/my_texture_frag_glsl.h"
#include "shaders/phong_frag_glsl.h"
#include "shaders/phong_vert_glsl.h"
#include "bubbleGenerator.h"


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

std::vector<glm::vec3> controlPoints = {
        {0, 0, -30},
        {30, 0, -30},
        {30, 0, 0},
        {0, 0, 0},
        {-30, 0, 0},
        {-30, 0, 30},
        {0, 0, 30},
        {30, 0, 30},
        {30, 0, 0},
        {0, 0, 0},
        {-30, 0, 0},
        {-30, 0, -30},
        {0, 0, -30}
};
std::vector<glm::vec3> points;
glm::vec3 position;
int step{0};
// These numbers are used to pass buffer data to OpenGL
GLuint vao = 0, vbo = 0;

glm::vec3 bezierPoint(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const float t) {
    float x = p0.x + ((p1.x - p0.x) * t);
    float z = p0.z + ((p1.z - p0.z) * t);
    glm::vec3 p01 = {x, 0, z};

    x = p1.x + ((p2.x - p1.x) * t);
    z = p1.z + ((p2.z - p1.z) * t);
    glm::vec3 p11 = {x, 0, z};

    x = p2.x + ((p3.x - p2.x) * t);
    z = p2.z + ((p3.z - p2.z) * t);
    glm::vec3 p21 = {x, 0, z};

    x = p01.x + ((p11.x - p01.x) * t);
    z = p01.z + ((p11.z - p01.z) * t);
    glm::vec3 p02 = {x, 0, z};

    x = p11.x + ((p21.x - p11.x) * t);
    z = p11.z + ((p21.z - p11.z) * t);
    glm::vec3 p12 = {x, 0, z};

    x = p02.x + ((p12.x - p02.x) * t);
    z = p02.z + ((p12.z - p02.z) * t);
    return {x, 0, z};
}



bool Boat::update(Scene &scene, float dt) {

//    if(prevCamY > 0 && scene.camera->position.y < 0){
//        shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, my_texture_frag_glsl);
//    }
//    if(prevCamY < 0 && scene.camera->position.y > 0){
//        shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
//    }
//    prevCamY = scene.camera->position.y;
    if(animate) {
        age += dt;
        glm::vec3 nextPosition;

        position = bezierPoint(controlPoints.at(3 * step),
                               controlPoints.at(3 * step + 1),
                               controlPoints.at(3 * step + 2),
                               controlPoints.at(3 * step + 3), age / speed);
        nextPosition = bezierPoint(controlPoints.at(3 * step),
                                   controlPoints.at(3 * step + 1),
                                   controlPoints.at(3 * step + 2),
                                   controlPoints.at(3 * step + 3), (age + dt) / speed);

        // rotate the boat
        rotation.y = (float) (std::atan2(nextPosition.x - position.x, nextPosition.z - position.z) + M_PI_2);
        // rotate the boat on waves
        rotation.x = (float) (0.3 * std::sin(position.x * age / 10) - ppgso::PI / 2);

        if (controlPoints.at(3 * step + 3).x - 0.1 < position.x &&
            position.x < controlPoints.at(3 * step + 3).x + 0.1 &&
            controlPoints.at(3 * step + 3).z - 0.1 < position.z &&
            position.z < controlPoints.at(3 * step + 3).z + 0.1) {
            age = 0;
            step++;
        }
        if (step == 4) step = 0;
    }
    else{
        if(!generator){
//            mesh = std::make_unique<ppgso::Mesh>("boat_test.obj");
            auto gen =  std::make_unique<BubbleGenerator>();
            scene.objects.push_back(std::move(gen));
            generator = true;
        }
    }



    generateModelMatrix();

    return true;
}

void Boat::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // Set up material

    shader->setUniform("MaterialShininess", 200.0f);


    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("material.ambient", {0.5f, 0.5f, 0.5f});
    shader->setUniform("material.diffuse", {0.8f, 0.8f, 0.8f});
    shader->setUniform("material.specular", {0.9f, 0.9f, 0.9f});
    shader->setUniform("material.shininess", 100.0f);

    shader->setUniform("lights.count", 5);
    for (int i = 0; i < 5; i++) {
        shader->setUniform("lights.positions[" + std::to_string(i) + "]", scene.lights.positions[i]);
        shader->setUniform("lights.colors[" + std::to_string(i) + "]", scene.lights.colors[i]);
        shader->setUniform("lights.ranges[" + std::to_string(i) + "]", scene.lights.ranges[i]);
        if (scene.lights.strengths[i] < 0) {
            shader->setUniform("lights.strengths[" + std::to_string(i) + "]", 0.9f);
        }
        else {
            shader->setUniform("lights.strengths[" + std::to_string(i) + "]", scene.lights.strengths[i]);
        }
    }


    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();

}

void Boat::onClick(Scene &scene) {

    std::cout << "Boat clicked!" << std::endl;
    //explode(scene, position, {10.0f, 10.0f, 10.0f}, 0 );
}

