// Example gl_scene
// - Introduces the concept of a dynamic scene of objects
// - Uses abstract object interface for Update and Render steps
// - Creates a simple game scene with Ocean, Boat and Cloud objects
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
#include "UnderWatterTerrain.h"
#include "Ocean.h"
#include "space.h"
#include "Dolphin.h"
#include "Boat.h"
#include "Sand.h"
#include "PerlinNoise.h"
#include "Fish.h"
#include "Bubble.h"
#include "Rock.h"
#include "Rain.h"
#include "Filter.h"
#include "shaders/texture_vert_glsl.h"
#include "shaders/texture_frag_glsl.h"


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

        scene.lightDirection = {0, 30, 0};

        // Create a camera
        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 500.0f);
        camera->position.z = -15.0f;
        scene.camera = std::move(camera);
        auto ocean = std::make_unique<Ocean>();
        scene.objects.push_back(std::move(ocean));


        auto shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
        scene.shader = std::move(shader);

//        auto filter = std::make_unique<Filter>();
//        filter->position.y = 10;
//        scene.objects.push_back(std::move(filter));

        // Add ocean to the scene
        /*int i, j;
        i = -80;
        while(i <= 80){
            j = -80;
            while(j <= 80){
                auto ocean = std::make_unique<Ocean>();
                ocean->position.x = i;
                ocean->position.z = j;
                scene.objects.push_back(std::move(ocean));
                j += 80;
            }
            i += 80;
        }

        i = -80;
        while(i <= 80){
            j = -80;
            while(j <= 80){
                auto ocean = std::make_unique<Ocean>();
                ocean->position.x = i;
                ocean->position.z = j;
                ocean->position.y = -5;
                ocean->rotation.x = glm::radians(180.0f);
                scene.objects.push_back(std::move(ocean));
                j += 80;
            }
            i += 80;
        }*/


        // Make a generator of dolphins which will be swim and can have collision with boat
        auto generator =  std::make_unique<Generator>();
        generator->position.y = 10.0f;
        scene.objects.push_back(std::move(generator));

        // add boat to scene
        auto boat = std::make_unique<Boat>();
        boat->position = {-20, 0, -20};
        boat->scale = {0.005f, 0.005f, 0.005f};
        boat->rotation.x = -ppgso::PI/2;
        boat->rotation.y = ppgso::PI;
        scene.objects.push_back(std::move(boat));
        auto rain = std::make_unique<Rain>( glm::vec3{0, 0, 0},10,10,10  );
        scene.objects.push_back(std::move(rain));

        auto upperWatter = std::make_unique<UnderWatterTerrain>();
        upperWatter->position = {0, 40,0};
        scene.objects.push_back(std::move(upperWatter));

        auto underwaterterrain = std::make_unique<UnderWatterTerrain>();
        underwaterterrain->position = {0, -40,0};
        scene.objects.push_back(std::move(underwaterterrain));

        //auto shader = std::make_unique<ppgso::Shader>(light_vert_glsl, light_frag_glsl);
        //scene.shader = move(shader);

        /*i = -80;
        while(i <= 80){
            j = -80;
            while(j <= 80){
                auto sand = std::make_unique<Sand>();
                sand->position.x = i;
                sand->position.z = j;
                scene.objects.push_back(std::move(sand));
                j += 80;
            }
            i += 80;
        }
        for(int i = 0; i < 30; i++){
            auto rock = std::make_unique<Rock>();
            rock->position.x = glm::linearRand(-115.0f, 115.0f);
            rock->position.z = glm::linearRand(-115.0f, 115.0f);
            // rotate rock randomly
            rock->rotation.x = glm::linearRand(0.0f, 360.0f);
            rock->rotation.y = glm::linearRand(0.0f, 360.0f);
            rock->rotation.z = glm::linearRand(0.0f, 360.0f);
            scene.objects.push_back(std::move(rock));
        }*/

        auto bubble = std::make_unique<Bubble>(glm::translate(glm::mat4(1.0f), {2 * sin(0.2),1, (0.5) * cos(0.2)}), ((float) rand() / (float) RAND_MAX) * (45 - 35) + 35, 0.035, 0.05, 0.1);
        scene.objects.push_back(move(bubble));

    }

public:

    /*!
     * Construct custom game window
     */
    SceneWindow() : Window{"Underwater world", SIZE, SIZE} {
        /*
        ppgso::Shader quadShader = {texture_vert_glsl, texture_frag_glsl};
        ppgso::Mesh quadMesh = {"quad.obj"};
        scene.light_positions.clear();
        scene.light_positions.push_back(glm::vec3(5, 7, -13));
        scene.shader->setUniform("lights[0].color", glm::vec3(1, 0.5, 0.5));
*/
        //hideCursor();
        glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // Initialize OpenGL state
        // Enable Z-buffer
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
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
                    scene.camera->moveW = true;
                    break;
                case GLFW_KEY_S:
                    scene.camera->moveS = true;
                    break;
                case GLFW_KEY_A:
                    scene.camera->moveA = true;
                    break;
                case GLFW_KEY_D:
                    scene.camera->moveD = true;
                    break;
                case GLFW_KEY_Q:
                    scene.camera->moveQ = true;
                    break;
                case GLFW_KEY_E:
                    scene.camera->moveE = true;
                    break;
                case GLFW_KEY_UP:
                    scene.camera->rotateUp = true;
                    break;
                case GLFW_KEY_DOWN:
                    scene.camera->rotateDown = true;
                    break;
                case GLFW_KEY_LEFT:
                    scene.camera->rotateLeft = true;
                    break;
                case GLFW_KEY_RIGHT:
                    scene.camera->rotateRight = true;
                    break;
                case GLFW_KEY_2:
                    scene.camera->position = {0, -75, 0};
                    break;
                case GLFW_MOUSE_BUTTON_LEFT:
//                    scene.camera->rotation.z -= 0.1;
                    break;
                case GLFW_KEY_SPACE:
                    //scene.camera->enableAnimation = false;
                    break;
                default:
                    break;
            }
        }
        if(action == GLFW_RELEASE){
            switch (key) {
                case GLFW_KEY_W:
                    scene.camera->moveW = false;
                    break;
                case GLFW_KEY_S:
                    scene.camera->moveS = false;
                    break;
                case GLFW_KEY_A:
                    scene.camera->moveA = false;
                    break;
                case GLFW_KEY_D:
                    scene.camera->moveD = false;
                    break;
                case GLFW_KEY_Q:
                    scene.camera->moveQ = false;
                    break;
                case GLFW_KEY_E:
                    scene.camera->moveE = false;
                    break;
                case GLFW_KEY_UP:
                    scene.camera->rotateUp = false;
                    break;
                case GLFW_KEY_DOWN:
                    scene.camera->rotateDown = false;
                    break;
                case GLFW_KEY_LEFT:
                    scene.camera->rotateLeft = false;
                    break;
                case GLFW_KEY_RIGHT:
                    scene.camera->rotateRight = false;
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

        if(scene.camera->moveW){
            auto shiftVector = glm::normalize(scene.camera->position - (scene.camera->position - scene.camera->back));
            scene.camera->position -= shiftVector * glm::vec3{scene.camera->moveSpeed, 0, scene.camera->moveSpeed};
            scene.camera->back -= shiftVector * glm::vec3{scene.camera->moveSpeed, 0, scene.camera->moveSpeed};
        }
        if(scene.camera->moveS){
            auto shiftVector = glm::normalize(scene.camera->position - (scene.camera->position - scene.camera->back));
            scene.camera->position += shiftVector * glm::vec3{scene.camera->moveSpeed, 0, scene.camera->moveSpeed};
            scene.camera->back += shiftVector * glm::vec3{scene.camera->moveSpeed, 0, scene.camera->moveSpeed};
        }
        if(scene.camera->moveA){
            auto shiftVector = glm::normalize(scene.camera->position - (scene.camera->position - scene.camera->back));
            scene.camera->position += glm::normalize(glm::cross(shiftVector, scene.camera->up)) * glm::vec3{scene.camera->moveSpeed, 0, scene.camera->moveSpeed};
            scene.camera->back += glm::normalize(glm::cross(shiftVector, scene.camera->up)) * glm::vec3{scene.camera->moveSpeed, 0, scene.camera->moveSpeed};
        }
        if(scene.camera->moveD){
            auto shiftVector = glm::normalize(scene.camera->position - (scene.camera->position - scene.camera->back));
            scene.camera->position -= glm::normalize(glm::cross(shiftVector, scene.camera->up)) * glm::vec3{scene.camera->moveSpeed, 0, scene.camera->moveSpeed};
            scene.camera->back -= glm::normalize(glm::cross(shiftVector, scene.camera->up)) * glm::vec3{scene.camera->moveSpeed, 0, scene.camera->moveSpeed};
        }
        if(scene.camera->moveQ){
            scene.camera->position.y += scene.camera->moveSpeed;
            scene.camera->back.y += scene.camera->moveSpeed;
        }
        if(scene.camera->moveE){
            scene.camera->position.y -= scene.camera->moveSpeed;
            scene.camera->back.y -= scene.camera->moveSpeed;
        }
        if(scene.camera->rotateUp && scene.camera->rotation.x > -1.5){
            scene.camera->rotation.x -= scene.camera->rotateSpeed;
        }
        if(scene.camera->rotateDown && scene.camera->rotation.x < 1.5){
            scene.camera->rotation.x += scene.camera->rotateSpeed;
        }
        if(scene.camera->rotateLeft){
            scene.camera->rotation.y -= scene.camera->rotateSpeed;
        }
        if(scene.camera->rotateRight){
            scene.camera->rotation.y += scene.camera->rotateSpeed;
        }

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
