
#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <ppgso/ppgso.h>
#include "Fish.h"
#include "Camera.h"
#include "Scene.h"

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

const unsigned int SIZE = 900;

class MyWindow : public ppgso::Window {
private:
    double time = glfwGetTime();
    Scene scene;
    void initScene() {
        // Create a camera
        //scene.camera = std::make_unique<Camera>(45.0f, (float)width / height, 0.1f, 100.0f);
        scene.camera->position.z = 5.0f;
        scene.camera->position.y = 5.0f;
        scene.camera->position.x = 5.0f;
        // create a fish
        scene.objects.push_back(std::make_unique<Fish>());
    }

public:
    MyWindow() : Window{"Underwater world", SIZE, SIZE} {
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);
        // enable backface culling
        glEnable(GL_CULL_FACE);
        // enable blending
        glEnable(GL_BLEND);
        glFrontFace(GL_CCW);
        // enable polygon culling
        glCullFace(GL_BACK);
        initScene();
    };
    void onKey(int key, int scancode, int action, int mods) override {
        if (action == GLFW_PRESS) {
            scene.keyboard[key] = true;
        }
        if (action == GLFW_RELEASE) {
            scene.keyboard[key] = false;
        }
    }

    void onIdle() override {
        // Get current time
        double currentTime = glfwGetTime();
        // Get time elapsed since last frame
        double deltaTime = float(currentTime - time);
        // Update time
        time = currentTime;
        // Update scene
        scene.update(float(deltaTime));
        // Render scene
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