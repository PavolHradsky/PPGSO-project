#include <glm/gtc/random.hpp>
#include <ppgso/ppgso.h>

#include "generator.h"
#include "Dolphin.h"

int counter = 0;

bool Generator::update(Scene &scene, float dt) {
    // Accumulate time
    time += dt;

    // Add object to scene when time reaches certain level
    if (time > .3 and counter < 10) {
        float frequency = glm::linearRand(7.0f / 250.0f, 9.0f / 250.0f);
        auto obj = std::make_unique<Dolphin>(position, rotation,frequency);
        obj->position = position;
        obj->position.x += glm::linearRand(-10.0f, 10.0f);
        obj->position.y += 0;
        obj->position.z += 0;
        std::cout <<  obj->position.x << " " << obj->position.y << " " << obj->position.z << std::endl;
        scene.objects.push_back(move(obj));
        time = 0;
        counter += 1;
    }

    return true;
}

void Generator::render(Scene &scene) {
    // Generator will not be rendered
}
