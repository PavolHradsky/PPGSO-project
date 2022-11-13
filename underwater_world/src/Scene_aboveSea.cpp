
#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include "Scene.h"
#include "Camera.h"
#include "Water.h"
#include "Dolphin.h"

const unsigned int SIZE = 512;

class MyWindow : public ppgso::Window {
private:
    Scene scene;
    float time = (float) glfwGetTime();

    void initScene() {
        scene.objects.clear();

        scene.lightDirection = {0, 0, 1};

        auto camera = std::make_unique<Camera>();
        camera->position = {0, 0, -10};
        scene.camera = std::move(camera);

        auto water = std::make_unique<Water>();
        water->position = {0, 0, 10};
        scene.objects.push_back(std::move(water));

        auto dolphin = std::make_unique<Dolphin>();
        scene.objects.push_back(std::move(dolphin));
    }
public:
    MyWindow() : Window{"Test Project", SIZE, SIZE} {
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
//        glDepthFunc(GL_LESS);
        glDepthFunc(GL_LEQUAL);
        // enable backface culling
        glEnable(GL_CULL_FACE);
        // enable blending
        glEnable(GL_BLEND);
        glFrontFace(GL_CCW);
        // enable polygon culling
        glCullFace(GL_BACK);

        initScene();

    };

    void onIdle() override {
        float dt = (float) glfwGetTime() - time;
        time = (float) glfwGetTime();
        scene.update(dt);
        scene.render();

    }
};


int main() {
    // Create our window
    auto window = MyWindow{};

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}