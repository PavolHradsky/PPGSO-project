
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

    bool animate = true;
    float time = (float) glfwGetTime();

    void initScene() {
        scene.objects.clear();

        scene.lightDirection = {0, 0, 1};

        auto camera = std::make_unique<Camera>();
        //camera->position = {0, 0, -10};
        scene.camera = std::move(camera);

        auto water = std::make_unique<Water>();
        //water->position = {0, 10, 0};
        scene.objects.push_back(std::move(water));

        auto dolphin = std::make_unique<Dolphin>();
        scene.objects.push_back(std::move(dolphin));
        // create terrain and add it to scene
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

    void onKey(int key, int scancode, int action, int mods) override {
        scene.keyboard[key] = action;
        if (action == GLFW_PRESS) {
            switch (key) {
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose(window, GL_TRUE);
                    break;
                case GLFW_KEY_R:
                    initScene();
                    break;
                case GLFW_KEY_P && GLFW_PRESS:
                    animate = !animate;
                    break;
                case GLFW_KEY_W:
                    scene.camera->position.z += 0.1;
                    break;
                case GLFW_KEY_S:
                    scene.camera->position.z -= 0.1;
                    break;
                case GLFW_KEY_A:
                    scene.camera->position.x -= 0.1;
                    break;
                case GLFW_KEY_D:
                    scene.camera->position.x += 0.1;
                    break;
                case GLFW_KEY_Q:
                    scene.camera->position.y += 0.1;
                    break;
                case GLFW_KEY_E:
                    scene.camera->position.y -= 0.1;
                    break;
                case GLFW_KEY_UP:
                    if (scene.lightDirection.z < 1) {
                        scene.lightDirection.z += 0.1;
                    }
                case GLFW_KEY_DOWN:
                    if (scene.lightDirection.z > -1) {
                        scene.lightDirection.z -= 0.1;
                    }
                case GLFW_KEY_LEFT:
                    if (scene.lightDirection.x > -1) {
                        scene.lightDirection.x -= 0.1;
                    }
                case GLFW_KEY_RIGHT:
                    if (scene.lightDirection.x < 1) {
                        scene.lightDirection.x += 0.1;
                    }
                    break;
                default:
                    break;
            }
        }
    }


    // display objects
    void onIdle() override {
        // Get current time
        float currentTime = (float) glfwGetTime();
        // Compute time difference between current and last frame
        float dt = currentTime - time;
        // Remember current time for next frame
        time = currentTime;

        // Update scene
        scene.update(dt);
        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // draw water object



        glBegin(GL_TRIANGLES);
        // Render the scene
        scene.render();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

};


int main() {
    // Create our window
    auto window = MyWindow{};

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}