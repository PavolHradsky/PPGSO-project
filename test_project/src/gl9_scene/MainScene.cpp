#include <list>
#include <ppgso/ppgso.h>
#include "camera.h"
#include "scene.h"
#include "generator.h"
#include "UnderWatterTerrain.h"
#include "Ocean.h"
#include "Cloud.h"
#include "Dolphin.h"
#include "Boat.h"
#include "Sand.h"
#include "Fish.h"
#include "Rock.h"
#include "Sun.h"
#include "Shadow.h"
#include "Filter.h"
#include "Seaweed.h"
#include "LightHouse.h"
#include "shaders/texture_vert_glsl.h"
#include "shaders/texture_frag_glsl.h"
#include "shaders/my_texture_frag_glsl.h"
#include "shaders/phong_vert_glsl.h"
#include "shaders/phong_frag_glsl.h"
#include "shaders/my_phong_frag_glsl.h"
#include "Light.h"
#include "Chest.h"
#include "Star.h"

const unsigned int SIZE = 980;

/*!
 * Custom windows for our simple game
 */
class SceneWindow : public ppgso::Window {
private:
    Scene scene;
    bool animate = true;
    glm::vec3 points;
    std::vector<float> vertices;
//    void switchedScene(){
//        auto fish = std::make_unique<Fish>();
//        if (fish) {
//            fish->animate = true;
//        }
//        scene.camera->position = {0, -50, 0};
//    }
    /*!
     * Reset and initialize the game scene
     * Creating unique smart pointers to objects that are stored in the scene object list
     */
    void initScene() {
        scene.objects.clear();


//        scene.lightDirection = {0, -30, 0};
        scene.lightDirection = {0.25 , 1, 0.5};

        // Create a camera
        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 500.0f);
        camera->position.z = -15.0f;
        scene.camera = std::move(camera);
        scene.camera->moveW = false;
        scene.camera->moveS = false;
        scene.camera->moveA = false;
        scene.camera->moveD = false;
        scene.camera->moveQ = false;
        scene.camera->moveE = false;
        scene.camera->rotateUp = false;
        scene.camera->rotateDown = false;
        scene.camera->rotateLeft = false;
        scene.camera->rotateRight = false;

        // Add ocean to the scene
        int i, j;
        i = -80;
        while (i <= 80) {
            j = -80;
            while (j <= 80) {
                auto ocean = std::make_unique<Ocean>();
                ocean->position.x = i;
                ocean->position.z = j;
                scene.objects.push_back(std::move(ocean));
                j += 80;
            }
            i += 80;
        }


        // Make a generator of dolphins which will be swim and can have collision with boat
        auto generator = std::make_unique<Generator>();
        generator->counter = 0;
        generator->position.y = 10.0f;
        scene.objects.push_back(std::move(generator));

        // add boat to scene
        auto boat = std::make_unique<Boat>();
        boat->position = {-20, 0, -20};
        boat->Boatposition = {-20, 0, -20};
        boat->scale = {0.005f, 0.005f, 0.005f};
        boat->rotation.x = -ppgso::PI / 2;
        boat->rotation.y = ppgso::PI;
        boat->animate = true;
        scene.objects.push_back(std::move(boat));

        auto sun = std::make_unique<Sun>();
        scene.objects.push_back(std::move(sun));

        for (int i = 0; i < 20; i++) {
            auto cloud = std::make_unique<Cloud>();
            cloud->position.y = 180;
            cloud->position.x += glm::linearRand(-120.0f, 120.0f);
            cloud->position.z += glm::linearRand(-120.0f, 120.0f);
            cloud->scale *= glm::linearRand(0.5f, 1.5f);
            cloud->rotation.y = glm::linearRand(0.0f, 2 * ppgso::PI);
            scene.objects.push_back(std::move(cloud));
        }

        auto lightHouse = std::make_unique<LightHouse>();
        lightHouse->position = {50, 0, 50};
        lightHouse->scale *= 10;
        scene.objects.push_back(std::move(lightHouse));


//        auto treasure = std::make_unique<Treasure>();
//        treasure->position = {-30, -78, -30};
//        treasure->scale = {0.9f, 0.9f, 0.9f};
//        scene.objects.push_back(std::move(treasure));

//        auto upperWatter = std::make_unique<UnderWatterTerrain>();
//        upperWatter->position = {0, 100, 0};
//        upperWatter->scale.y = 1;
//        scene.objects.push_back(std::move(upperWatter));

        auto sky = std::make_unique<Filter>();
        sky->position = {0, 200, 0};
        sky->scale = {3, 3, 3};
        sky->rotation.x = glm::radians(180.0f);
        scene.objects.push_back(std::move(sky));

        auto wall1 = std::make_unique<Filter>();
        wall1->position = {0, 50, -120};
        wall1->scale = {3, 3, 3};
        wall1->rotation.x = glm::radians(90.0f);
        scene.objects.push_back(std::move(wall1));

        auto wall2 = std::make_unique<Filter>();
        wall2->position = {0, 50, 120};
        wall2->scale = {3, 3, 3};
        wall2->rotation.x = glm::radians(90.0f);
        wall2->rotation.y = glm::radians(180.0f);
        scene.objects.push_back(std::move(wall2));

        auto wall3 = std::make_unique<Filter>();
        wall3->position = {120, 50, 0};
        wall3->scale = {3, 3, 3};
        wall3->rotation.x = glm::radians(90.0f);
        wall3->rotation.y = glm::radians(90.0f);
        scene.objects.push_back(std::move(wall3));

        auto wall4 = std::make_unique<Filter>();
        wall4->position = {-120, 50, 0};
        wall4->scale = {3, 3, 3};
        wall4->rotation.x = glm::radians(90.0f);
        wall4->rotation.y = glm::radians(-90.0f);
        scene.objects.push_back(std::move(wall4));

        Filter::shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
        Ocean::shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);


        scene.global_lighting_on = false;
        // lighthouse light
        scene.lights.positions[0] = {50, 20, 50};
        scene.lights.colors[0] = {1, 1, 1};
        scene.lights.ranges[0] = 40;
        scene.lights.strengths[0] = 6;

        // underwater light
        scene.lights.positions[1] = {50, -40, 50};
        scene.lights.colors[1] = {1, 1, 1};
        scene.lights.ranges[1] = 70;
        scene.lights.strengths[1] = 6;

        // sun
        scene.lights.positions[2] = {0, 190, 0};
        scene.lights.colors[2] = {1, 1, 1};
        scene.lights.ranges[2] = 300;
        scene.lights.strengths[2] = 5;

        // boat light
        scene.lights.positions[3] = {0, 0, 0};
        scene.lights.colors[3] = {1, 1, 1};
        scene.lights.ranges[3] = 50;
        scene.lights.strengths[3] = 3;

        // underwater boat light
        scene.lights.positions[4] = {-20, -60, -20};
        scene.lights.colors[4] = {1, 1, 1};
        scene.lights.ranges[4] = 150;
        scene.lights.strengths[4] = 3;

        scene.lights.count = 5;
    }

    void initScene2(){
        scene.objects.clear();


        // Create a camera
        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 500.0f);
        camera->position.z = -15.0f;
        camera->position.y = -15.0f;
        scene.camera = std::move(camera);
        scene.camera->moveW = false;
        scene.camera->moveS = false;
        scene.camera->moveA = false;
        scene.camera->moveD = false;
        scene.camera->moveQ = false;
        scene.camera->moveE = false;
        scene.camera->rotateUp = false;
        scene.camera->rotateDown = false;
        scene.camera->rotateLeft = false;
        scene.camera->rotateRight = false;

        int j;
        int i = -80;
        while (i <= 80) {
            j = -80;
            while (j <= 80) {
                auto ocean = std::make_unique<Ocean>();
                ocean->position.x = i;
                ocean->position.z = j;
                ocean->position.y = -5;
                ocean->rotation.x = glm::radians(180.0f);
                scene.objects.push_back(std::move(ocean));
                j += 80;
            }
            i += 80;
        }


        // Make a generator of dolphins which will be swim and can have collision with boat
        auto generator = std::make_unique<Generator>();
        generator->position.y = 10.0f;
        generator->counter = 0;
        scene.objects.push_back(std::move(generator));

        // add boat to scene
        auto drownedBoat = std::make_unique<Boat>();
        drownedBoat->position = {-20, -80, -20};
        drownedBoat->scale = {0.03f, 0.03f, 0.03f};
        drownedBoat->rotation.x = -ppgso::PI / 3;
        drownedBoat->animate = false;
        scene.objects.push_back(std::move(drownedBoat));

        auto wall1 = std::make_unique<Filter>();
        wall1->position = {0, 50, -120};
        wall1->scale = {3, 3, 3};
        wall1->rotation.x = glm::radians(90.0f);
        scene.objects.push_back(std::move(wall1));

        auto wall2 = std::make_unique<Filter>();
        wall2->position = {0, 50, 120};
        wall2->scale = {3, 3, 3};
        wall2->rotation.x = glm::radians(90.0f);
        wall2->rotation.y = glm::radians(180.0f);
        scene.objects.push_back(std::move(wall2));

        auto wall3 = std::make_unique<Filter>();
        wall3->position = {120, 50, 0};
        wall3->scale = {3, 3, 3};
        wall3->rotation.x = glm::radians(90.0f);
        wall3->rotation.y = glm::radians(90.0f);
        scene.objects.push_back(std::move(wall3));

        auto wall4 = std::make_unique<Filter>();
        wall4->position = {-120, 50, 0};
        wall4->scale = {3, 3, 3};
        wall4->rotation.x = glm::radians(90.0f);
        wall4->rotation.y = glm::radians(-90.0f);
        scene.objects.push_back(std::move(wall4));

        Filter::shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, my_phong_frag_glsl);
        Ocean::shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, my_phong_frag_glsl);


        auto sand = std::make_unique<Sand>();


        for (int k = 0; k < 50; k++) {
            auto rock = std::make_unique<Rock>();
            auto seaweed = std::make_unique<Seaweed>();
            auto shadow = std::make_unique<Shadow>();

            rock->position.x = glm::linearRand(-115.0f, 115.0f);
            rock->position.z = glm::linearRand(-115.0f, 115.0f);
            seaweed->position.x = glm::linearRand(-115.0f, 115.0f);
            seaweed->position.z = glm::linearRand(-115.0f, 115.0f);
            shadow->position.x = rock->position.x;
            shadow->position.z = rock->position.z;

            float x = rock->position.x;
            float z = rock->position.z;
            // find nearest x, z in sand->bezier.vertices
            for (auto &point: sand->bezier.vertices) {
                if (x - 4 < point.x && point.x < x + 4) {
                    if (z - 4 < point.z && point.z < z + 4) {
                        rock->position.y = point.y;
                        shadow->position.y = point.y;
                        break;
                    }
                }
            }

            x = seaweed->position.x;
            z = seaweed->position.z;
            // find nearest x, z in sand->bezier.vertices
            for (auto &point: sand->bezier.vertices) {
                if (x - 4 < point.x && point.x < x + 4) {
                    if (z - 4 < point.z && point.z < z + 4) {
                        seaweed->position.y = point.y;
                        break;
                    }
                }
            }

            // rotate rock randomly
            rock->rotation.x = glm::linearRand(0.0f, 360.0f);
            rock->rotation.y = glm::linearRand(0.0f, 360.0f);
            rock->rotation.z = glm::linearRand(0.0f, 360.0f);
            scene.objects.push_back(std::move(seaweed));
            scene.objects.push_back(std::move(rock));
            scene.objects.push_back(std::move(shadow));
        }

        scene.objects.push_back(std::move(sand));

        // add chest to scene
        auto treasure = std::make_unique<Chest>();
        treasure->position = {-30, -77, -30};
        treasure->rotation.z = ppgso::PI;
        scene.objects.push_back(std::move(treasure));


        scene.global_lighting_on = false;
        // lighthouse light
        scene.lights.positions[0] = {50, 20, 50};
        scene.lights.colors[0] = {1, 1, 1};
        scene.lights.ranges[0] = 40;
        scene.lights.strengths[0] = 6;

        // underwater light
        scene.lights.positions[1] = {50, -40, 50};
        scene.lights.colors[1] = {1, 1, 1};
        scene.lights.ranges[1] = 70;
        scene.lights.strengths[1] = 6;

        // sun
        scene.lights.positions[2] = {0, 190, 0};
        scene.lights.colors[2] = {1, 1, 1};
        scene.lights.ranges[2] = 300;
        scene.lights.strengths[2] = 5;

        // boat light
        scene.lights.positions[3] = {0, 0, 0};
        scene.lights.colors[3] = {1, 1, 1};
        scene.lights.ranges[3] = 50;
        scene.lights.strengths[3] = 3;

        // underwater boat light
        scene.lights.positions[4] = {-20, -60, -20};
        scene.lights.colors[4] = {1, 1, 1};
        scene.lights.ranges[4] = 150;
        scene.lights.strengths[4] = 3;

        scene.lights.count = 5;
    }

public:

    /*!
     * Construct custom game window
     */
    SceneWindow() : Window{"Underwater world", SIZE, SIZE} {
        glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

        // Initialize OpenGL state
        // Enable Z-buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_FOG);

        // Enable polygon culling
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        initScene();
    }

    void onKey(int key, int scancode, int action, int mods) override {
        scene.keyboard[key] = action;
        if (action == GLFW_PRESS) {
            switch (key) {
                case GLFW_KEY_1:
                    initScene();
                    break;
                case GLFW_KEY_2:
                    initScene2();
                    break;
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
                case GLFW_KEY_3:
                    // move camera to lighthouse
                    scene.camera->moveTo({7.69656, 4.88165, 7.65431},{18.6847, 9.22027, -2.41117}, {0.000000, 10, 0.000000});
                    scene.camera->start_looking_at = {60, 4.39217, 20};
                    scene.camera->end_looking_at = {22.2458 , -1.09873, 11.0905};
                    scene.camera->t = -1;
                    scene.camera->animate = true;
                    action = GLFW_RELEASE;
                    break;
                case GLFW_KEY_4:
                    // move camera to sun
                    scene.camera->moveTo({-22.2458 , -1.09873, -11.0905},{40, 190, 0}, {0.000000, 0, 0.000000});
                    scene.camera->start_looking_at = {-22.2458 , -1.09873, -11.0905};
                    scene.camera->end_looking_at = {0, 190, 0};
                    scene.camera->t = -1;
                    scene.camera->animate = true;
                    action = GLFW_RELEASE;
                    break;
                case GLFW_KEY_5:
                    // move camera to boat
                    scene.camera->moveTo({-18.6847, 9.22027, -2.41117},{60, -60, 0}, {0.000000, 0, 0.000000});
                    scene.camera->start_looking_at = {-22.2458 , -60, -11.0905};
                    scene.camera->end_looking_at = {-20, -60, 0};
                    scene.camera->t = -1;
                    scene.camera->animate = true;
                    action = GLFW_RELEASE;
                    break;
                case GLFW_KEY_6:
                    // move camera to treasure
                    scene.camera->moveTo({-20, -60, -20},{-20, -75, -30}, {0.000000, 0, 0.000000});
                    scene.camera->start_looking_at = {-20, -60, -20};
                    scene.camera->end_looking_at = {-30, -77, -30};
                    scene.camera->t = -1;
                    scene.camera->animate = true;
                    action = GLFW_RELEASE;
                    break;
                case GLFW_KEY_7:
                    // follow boat on ocean TODO
                    scene.camera->enableAnimationBoat = true;
                    action = GLFW_RELEASE;
                    break;
                case GLFW_KEY_N:
                    scene.nightSwitch = !scene.nightSwitch;
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
