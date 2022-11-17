// Example gl_scene
// - Introduces the concept of a dynamic scene of objects
// - Uses abstract object interface for Update and Render steps
// - Creates a simple game scene with Ocean, Boat and Space objects
// - Contains a generator object that does not render but adds Asteroids to the scene
// - Some objects use shared resources and all object deallocations are handled automatically
// - Controls: LEFT, RIGHT, "R" to reset, SPACE to fire

#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>

#include "camera.h"
#include "scene.h"
#include "generator.h"
#include "Ocean.h"
#include "space.h"
#include "Dolphin.h"

const unsigned int SIZE = 980;

/*!
 * Custom windows for our simple game
 */
class SceneWindow : public ppgso::Window {
private:
    Scene scene;
    bool animate = true;

    /*!
     * Reset and initialize the game scene
     * Creating unique smart pointers to objects that are stored in the scene object list
     */
    void initScene() {
        scene.objects.clear();

        // Create a camera
        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
        camera->position.z = -15.0f;
        scene.camera = move(camera);

        // Add ocean to the scene
        auto ocean = std::make_unique<Ocean>();

        glm::vec3 position_dolphin1 = {ocean->position.x, ocean->position.y, 0};
        glm::vec3 position_dolphin2 = {position_dolphin1.x+13, position_dolphin1.y+5, -3};

        scene.objects.push_back(move(ocean));
        glm::vec3 rotation = {3 * ppgso::PI / 2, 0, 2 * ppgso::PI / 3};
/*
        for (int i = 0; i < 10; i++)
        {
            float fre = glm::linearRand(3, 8);
            glm::vec3 pos = glm::vec3{glm::linearRand(ocean->position.x-0.5, ocean->position.x+0.5), glm::linearRand(ocean->position.y-1, ocean->position.y+1),  glm::linearRand(ocean->position.z-5, ocean->position.z+5)};
            auto dolphin = std::make_unique<Dolphin>(pos,  rotation, fre / 250.0f);
            dolphin->scale = {0.01f, 0.01f, 0.01f};
            scene.objects.push_back(move(dolphin));
        }

*/

        float freq1 = 7.0f / 250.0f;

        auto dolphin1 = std::make_unique<Dolphin>(position_dolphin1, rotation, freq1);
        dolphin1->scale = {0.01f, 0.01f, 0.01f};
        scene.objects.push_back(move(dolphin1));

        float freq2 = 10.0f / 250.0f;
        auto dolphin2 = std::make_unique<Dolphin>(position_dolphin2, rotation, freq2);
        dolphin2->scale = {0.005f, 0.005f, 0.005f};
        scene.objects.push_back(move(dolphin2));
    }

public:
    /*!
     * Construct custom game window
     */
    SceneWindow() : Window{"gl9_scene", SIZE, SIZE} {
        //hideCursor();
        glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

        // Initialize OpenGL state
        // Enable Z-buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        // Enable polygon culling
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        initScene();
    }

    /*!
     * Handles pressed key when the window is focused
     * @param key Key code of the key being pressed/released
     * @param scanCode Scan code of the key being pressed/released
     * @param action Action indicating the key state change
     * @param mods Additional modifiers to consider
     */
//  void onKey(int key, int scanCode, int action, int mods) override {
//    scene.keyboard[key] = action;
//
//    // Reset
//    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
//      initScene();
//    }
//
//    // Pause
//    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
//      animate = !animate;
//    }
//  }

    void onKey(int key, int scancode, int action, int mods) override {
        scene.keyboard[key] = action;
        if (action == GLFW_PRESS) {
            switch (key) {
                case GLFW_KEY_1:
                    scene.camera->mode = Camera::FOLLOW;
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
                    scene.camera->position.z += 1;
                    break;
                case GLFW_KEY_S:
                    scene.camera->position.z -= 1;
                    break;
                case GLFW_KEY_A:
                    scene.camera->position.x += 1;
                    break;
                case GLFW_KEY_D:
                    scene.camera->position.x -= 1;
                    break;
                case GLFW_KEY_Q:
                    scene.camera->position.y += 1;
                    break;
                case GLFW_KEY_E:
                    scene.camera->position.y -= 1;
                    break;
                case GLFW_KEY_UP:
                    scene.positionOcean.z -= 1;
                case GLFW_KEY_DOWN:
                    scene.positionOcean.z += 1;
                case GLFW_KEY_LEFT:
                    scene.positionOcean.x -= 1;
                case GLFW_KEY_RIGHT:
                    scene.positionOcean.x -= 1;
                case GLFW_KEY_SPACE:

                    break;
                default:
                    break;
            }
        }
    }

    /*!
     * Handle cursor position changes
     * @param cursorX Mouse horizontal position in window coordinates
     * @param cursorY Mouse vertical position in window coordinates
     */
    void onCursorPos(double cursorX, double cursorY) override {
        scene.cursor.x = cursorX;
        scene.cursor.y = cursorY;
    }

    /*!
     * Handle cursor buttons
     * @param button Mouse button being manipulated
     * @param action Mouse bu
     * @param mods
     */
    void onMouseButton(int button, int action, int mods) override {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            scene.cursor.left = action == GLFW_PRESS;

            if (scene.cursor.left) {
                // Convert pixel coordinates to Screen coordinates
                double u = (scene.cursor.x / width - 0.5f) * 2.0f;
                double v = -(scene.cursor.y / height - 0.5f) * 2.0f;

                // Get mouse pick vector in world coordinates
                auto direction = scene.camera->cast(u, v);
                auto position = scene.camera->position;

                // Get all objects in scene intersected by ray
                auto picked = scene.intersect(position, direction);

                // Go through all objects that have been picked
                for (auto &obj: picked) {
                    // Pass on the click event
                    obj->onClick(scene);
                }
            }
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            scene.cursor.right = action == GLFW_PRESS;
        }
    }

    /*!
     * Window update implementation that will be called automatically from pollEvents
     */
    void onIdle() override {
        // Track time
        static auto time = (float) glfwGetTime();

        // Compute time delta
        float dt = animate ? (float) glfwGetTime() - time : 0;

        time = (float) glfwGetTime();

        // Set gray background
        glClearColor(.5f, .5f, .5f, 0);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and render all objects
        scene.update(dt);
        scene.render();
    }
};

int main() {
    // Initialize our window
    SceneWindow window;

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}
