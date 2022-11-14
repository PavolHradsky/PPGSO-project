
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

const unsigned int SIZE = 900;

class MyWindow : public ppgso::Window {
private:
    Scene scene;

    bool animate = true;
    float time = (float) glfwGetTime();

    float color_r = 1.0f;
    float color_g = 0.0f;
    float color_b = 0.0f;
    float color_a = 1.0f;

    void initScene() {
        scene.objects.clear();

        scene.lightDirection = {0, 0, 1};

        auto camera = std::make_unique<Camera>();
        //camera->position = {0, 0, -10};
        scene.camera = move(camera);
        glm::vec3 position = {-32.2116,-2.79839,-14.8765};
        auto water = std::make_unique<Water>("models/water.obj");
        position.y = scene.getHeight(position.x, position.z);
        scene.objects.push_back(move(water));

        auto dolphin = std::make_unique<Dolphin>();
        dolphin->position = {0, 100, 0};
        dolphin->scale = {100, 100, 100};
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
                    // add object to scene
                    color_r = 1.0f;
                    color_g = 0.0f;
                    color_b = 0.0f;
                    break;
                case GLFW_KEY_S:
                    scene.camera->position.z -= 0.1;
                    color_r = 0.0f;
                    color_g = 1.0f;
                    color_b = 0.0f;
                    break;
                case GLFW_KEY_A:
                    scene.camera->position.x -= 0.1;
                    color_r = 0.0f;
                    color_g = 0.0f;
                    color_b = 1.0f;
                    break;
                case GLFW_KEY_D:
                    scene.camera->position.x += 0.1;
                    color_r = 1.0f;
                    color_g = 1.0f;
                    color_b = 0.0f;
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
                    case GLFW_KEY_1:
                        scene.camera->mode = Camera::FOLLOW;
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


        // blue background
        glClearColor(color_r, color_g, color_b, color_a);
        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Update scene
        scene.update(dt);
        // draw water object
        scene.render();
        //glfwPollEvents();
    }

};


int main() {
    // Create our window
    auto window = MyWindow{};

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}