#ifndef UNDERWATER_WORLD_SCENE_H
#define UNDERWATER_WORLD_SCENE_H
#include <vector>
#include "Object.h"

class Scene {
public:

    void update(float time);

    void render();

    std::vector<Object *> intersect(glm::vec3 origin, glm::vec3 direction);

};

#endif //UNDERWATER_WORLD_SCENE_H