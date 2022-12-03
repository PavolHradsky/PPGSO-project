#ifndef _PPGSO_SCENE_H
#define _PPGSO_SCENE_H

#include <memory>
#include <map>
#include <list>

#include "object.h"
#include "camera.h"

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
    int LIGHT_COUNT = 3;
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

    glm::vec3 sandControlPoints[121]{
            {-120,  -80, -120}, {-120,  -80, -96}, {-120,  -80, -72}, {-120,  -80, -48}, {-120,  -80, -24}, {-120,  -80, 0}, {-120,  -80, 24}, {-120,  -80, 48}, {-120,  -70, 72}, {-120,  -60, 96}, {-120, -60, 120},
            { -96,  -80, -120}, { -96,  -80, -96}, { -96,  -80, -72}, { -96,  -80, -48}, { -96,  -80, -24}, { -96,  -80, 0}, { -96,  -80, 24}, { -96,  -80, 48}, { -96,  -70, 72}, { -96,  -70, 96}, {-96,  -60, 120},
            { -72,  -80, -120}, { -72,  -75, -96}, { -72,  -80, -72}, { -72,  -80, -48}, { -72,  -80, -24}, { -72,  -80, 0}, { -72,  -80, 24}, { -72,  -80, 48}, { -72,  -80, 72}, { -72,  -70, 96}, {-72,  -70, 120},
            { -48,  -75, -120}, { -48,  -65, -96}, { -48,  -75, -72}, { -48,  -80, -48}, { -48,  -80, -24}, { -48,  -80, 0}, { -48,  -80, 24}, { -48,  -80, 48}, { -48,  -80, 72}, { -48,  -80, 96}, {-48,  -80, 120},
            { -24,  -65, -120}, { -24,  -60, -96}, { -24,  -65, -72}, { -24,  -75, -48}, { -24,  -80, -24}, { -24,  -80, 0}, { -24,  -80, 24}, { -24,  -80, 48}, { -24,  -80, 72}, { -24,  -80, 96}, {-24,  -80, 120},
            {   0,  -75, -120}, {   0,  -65, -96}, {   0,  -75, -72}, {   0,  -80, -48}, {   0,  -80, -24}, {   0,  -80, 0}, {   0,  -80, 24}, {   0,  -60, 48}, {   0,  -80, 72}, {   0,  -80, 96}, {0, -80, 120},
            {  24,  -80, -120}, {  24,  -75, -96}, {  24,  -80, -72}, {  24,  -80, -48}, {  24,  -80, -24}, {  24,  -80, 0}, {  24,  -60, 24}, {  24,  -40, 48}, {  24,  -60, 72}, {  24,  -80, 96}, {24, -80, 120},
            {  48,  -80, -120}, {  48,  -80, -96}, {  48,  -80, -72}, {  48,  -80, -48}, {  48,  -80, -24}, {  48,  -60, 0}, {  48,  -40, 24}, {  48,  -30, 48}, {  48,  -40, 72}, {  48,  -60, 96}, {48, -80, 120},
            {  72,  -80, -120}, {  72,  -80, -96}, {  72,  -80, -72}, {  72,  -80, -48}, {  72,  -80, -24}, {  72,  -80, 0}, {  72,  -60, 24}, {  72,  -40, 48}, {  72,  -60, 72}, {  72,  -80, 96}, {72, -80, 120},
            {  96,  -80, -120}, {  96,  -80, -96}, {  96,  -80, -72}, {  96,  -80, -48}, {  96,  -80, -24}, {  96,  -80, 0}, {  96,  -80, 24}, {  96,  -60, 48}, {  96,  -80, 72}, {  96,  -80, 96}, {96, -80, 120},
            { 120,  -80, -120}, { 120,  -80, -96}, { 120,  -80, -72}, { 120,  -80, -48}, { 120,  -80, -24}, { 120,  -80, 0}, { 120,  -80, 24}, { 120,  -80, 48}, { 120,  -80, 72}, { 120,  -80, 96}, {120, -80, 120}
    };
};

#endif // _PPGSO_SCENE_H
