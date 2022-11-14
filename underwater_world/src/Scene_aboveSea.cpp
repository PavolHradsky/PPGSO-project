
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
    float color_r = 1.0f;
    float color_g = 0.0f;
    float color_b = 0.0f;
    float color_a = 1.0f;

    void buffer_init() {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        quadTexture.bind();
        unsigned int framebufferTexture;
        glGenTextures(1, &framebufferTexture);
        glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

        // Initialize framebuffer, its color texture (the sphere will be rendered to it) and its render buffer for depth info storage
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // Set up render buffer that has a depth buffer and stencil buffer
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        // Associate the quadTexture with it
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, quadTexture.image.width, quadTexture.image.height);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, quadTexture.getTexture(), 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw std::runtime_error("Cannot create framebuffer!");
    }

    void initScene() {
        buffer_init();
        scene.objects.clear();

        scene.lightDirection = {0, 0, 1};

        auto camera = std::make_unique<Camera>();
        camera->position.x = 100.0f;
        camera->position.y = (float) width / (float) height;
        camera->position.z = 100.0f;
        scene.camera = move(camera);

        auto water = std::make_unique<Water>();
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

    void buffer_show() {
        resetViewport();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Clear the framebuffer
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto quadViewMatrix = glm::mat4{1.0f};
        quadViewMatrix = glm::lookAt(glm::vec3{0.0f, 0.0f, -0.8f},
                                     glm::vec3{0.0f, 1.0f, 0.0f} - glm::vec3{0.0f, 1.0f, -1.0f}, {0.0f, -1.0f, 0.0f});

        // Animate rotation of the quad
        auto quadModelMatrix = glm::mat4{1.0f};

        // Set shader inputs
        quadShader.use();
        quadShader.setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        quadShader.setUniform("ViewMatrix", quadViewMatrix);
        quadShader.setUniform("ModelMatrix", quadModelMatrix);
        quadShader.setUniform("Texture", quadTexture);
        quadMesh.render();
    }

    // display objects
    void onIdle() override {
        glViewport(0, 0, 1024, 1024);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glClearColor(0.0f, 0.0f, 0.0f, 0);
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

        // blue background
        //glClearColor(color_r, color_g, color_b, color_a);
        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        buffer_show();
    }

};


int main() {
    // Create our window
    auto window = MyWindow{};

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}