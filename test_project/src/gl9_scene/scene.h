#ifndef _PPGSO_SCENE_H
#define _PPGSO_SCENE_H

#include <memory>
#include <map>
#include <list>
#include "object.h"
#include "camera.h"
#include "Sun.h"

/*
 * Scene is an object that will aggregate all scene related data
 * Objects are stored in a list of objects
 * Keyboard and Mouse states are stored in a map and struct
 */
class Scene {
  public:
    // shader
    std::unique_ptr<ppgso::Shader> shader;

    // lights
    std::vector<glm::vec3> light_positions;
    glm::vec3 player_position{0, 0, 0};
    /*!
     * Update all objects in the scene
     * @param time
     */
    void update(float time);

    /*!
     * Render all objects in the scene
     */
    void render();

    /*!
     * Pick objects using a ray
     * @param position - Position in the scene to pick object from
     * @param direction - Direction to pick objects from
     * @return Objects - Vector of pointers to intersected objects
     */
    std::vector<Object*> intersect(const glm::vec3 &position, const glm::vec3 &direction);

    // Camera object
    std::unique_ptr<Camera> camera;

    // All objects to be rendered in scene
    std::list< std::unique_ptr<Object> > objects;
    // Keyboard state
    std::map< int, int > keyboard;

    // Lights, in this case using only simple directional diffuse lighting
    glm::vec3 lightDirection{-1.0f, -1.0f, -1.0f};
    bool nightSwitch = false;
    // Store cursor state
    struct {
      double x, y;
      bool left, right;
    } cursor;

    bool global_lighting_on = true;
    glm::vec3 globalPosition = {0,0,0};
    struct Lights {
        int count;
        glm::vec3 positions[100];
        glm::vec3 colors[100];
        float ranges[100];
        float strengths[100];
    };

    Lights lights;
    int rock_lights = 15;
    bool convolution = false;
    bool prevConvolution = false;
};

#endif // _PPGSO_SCENE_H
