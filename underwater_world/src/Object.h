#ifndef UNDERWATER_WORLD_OBJECT_H
#define UNDERWATER_WORLD_OBJECT_H

#include <glm/glm.hpp>

class Scene;

class Object {
public:
    Object() = default;

    Object(const Object &) = default;

    Object(Object &&) = default;

    virtual ~Object() = default;

    virtual bool update(Scene &scene, float dt) = 0;

    virtual void render(Scene &scene) = 0;

    virtual void onClick(Scene &scene) {};
    glm::vec3 position{0, 0, 0};
    glm::vec3 rotation{0, 0, 0};
    glm::vec3 scale{1, 1, 1};
    glm::mat4 modelMatrix{1};
protected:
    void generateModelMatrix();
};
#endif //UNDERWATER_WORLD_OBJECT_H