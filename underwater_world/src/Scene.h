#ifndef UNDERWATER_WORLD_SCENE_H
#define UNDERWATER_WORLD_SCENE_H

#include <vector>
#include <list>
#include "Object.h"
#include "Camera.h"

class Scene {
public:

    void update(float time);

    void render();

    std::unique_ptr<ppgso::Shader> shader;

    std::vector<Object *> intersect(const glm::vec3 &position, const glm::vec3 &direction);

    // Camera object
    std::unique_ptr<Camera> camera;

    // All objects to be rendered in Scene
    std::list<std::unique_ptr<Object> > objects;
    int imgHeight;
    int imgWidth;
    unsigned char *heightFramebuffer;
    // Keyboard state
    std::map<int, int> keyboard;

    // Lights, in this case using only simple directional diffuse lighting
    glm::vec3 lightDirection{-1.0f, -1.0f, -1.0f};
    glm::vec3 lightColor{1.0f, 1.0f, 1.0f};

    // Store cursor state
    struct {
        double x, y;
        bool left, right;
    } cursor;

    void setTargetPosition(const glm::vec3 &position, const glm::vec3 &rotation);

    float getHeight(float x, float y);
};

#endif //UNDERWATER_WORLD_SCENE_H