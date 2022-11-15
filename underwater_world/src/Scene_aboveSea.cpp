#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <ppgso/ppgso.h>
#include "Scene.h"
#include "Camera.h"
#include "Water.h"
#include "Dolphin.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

const unsigned int SIZE = 900;

class MyWindow : public ppgso::Window {
private:
    Scene scene;
    GLuint fbo = 1;
    GLuint rbo = 1;
    ppgso::Texture quadTexture = {1024, 1024};
    bool animate = true;
    float time = (float) glfwGetTime();
    ppgso::Shader quadShader = {texture_vert_glsl, texture_frag_glsl};
    ppgso::Mesh quadMesh = {"data/models/water.obj"};

    void initScene() {
        scene.objects.clear();

        scene.lightDirection = {0, 0, 1};

        auto camera = std::make_unique<Camera>();
        camera->position.x = 100.0f;
        camera->position.y = (float) width / (float) height;
        camera->position.z = 100.0f;
        scene.camera = move(camera);

        //auto water = std::make_unique<Water>();
        //scene.objects.push_back(move(water));

        auto dolphin = std::make_unique<Dolphin>();
        dolphin->position = {0, 100, 0};
        dolphin->scale = {100, 100, 100};
        scene.objects.push_back(std::move(dolphin));
        // create terrain and add it to scene
    }
/// Abstract renderable object interface
    class Renderable; // Forward declaration for Scene
    using Scene = std::list<std::unique_ptr<Renderable>>; // Type alias

    class Renderable {
    public:
        // Virtual destructor is needed for abstract interfaces
        virtual ~Renderable() = default;

        /// Render the object
        /// \param camera - Camera to use for rendering
        virtual void render(const Camera &camera) = 0;

        /// Update the object. Useful for specifing animation and behaviour.
        /// \param dTime - Time delta
        /// \param scene - Scene reference
        /// \return - Return true to keep object in scene
        virtual bool update(float dTime, Scene &scene) = 0;
        virtual std::pair<bool, glm::vec3> is_sphere() = 0;
    };
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
    glm::vec3 random_vec3 (float mini, float maxi) {
        return {((float) rand() / (float) RAND_MAX) * (maxi - mini) + mini, ((float) rand() / (float) RAND_MAX) * (maxi - mini) + mini, ((float) rand() / RAND_MAX) * (maxi - mini) + mini};
    };

    glm::vec3 sameRandom_vec3 (float mini, float maxi) {
        float sur = ((float) rand() / (float) RAND_MAX) * (maxi - mini) + mini;
        return { sur, sur, sur};
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
                case GLFW_KEY_SPACE:
                    scene.objects.push_back(move(std::make_unique<Water>()));
                    //scene.objects.push_back(std::make_unique<Water>( random_vec3(2, 5), random_vec3(-3, 3), sameRandom_vec3(0.25, 0.5 )));
                    break;
                default:
                    break;
            }
        }
    }

    // display objects
    void onIdle() override {
        // Set gray background
        glClearColor(.1f, .1f, .1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Get current time
        float currentTime = (float) glfwGetTime();
        // Compute time difference between current and last frame
        float dt = currentTime - time;
        // Remember current time for next frame
        time = currentTime;
        // Update scene
        scene.update(dt);
        // draw water object
        scene.render();
        //glfwPollEvents();
        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Update all objects in scene
        // Because we need to delete while iterating this is implemented using c++ iterators
        // In most languages mutating the container during iteration is undefined behaviour

    }

};


int main() {
    // Create our window
    auto window = MyWindow{};

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}