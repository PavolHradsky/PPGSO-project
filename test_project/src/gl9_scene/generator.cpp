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
        auto obj = std::make_unique<Dolphin>();
        obj->speed = glm::linearRand(2.0f, 5.0f);

        obj->position.x += glm::linearRand(-40.0f, 40.0f);
        obj->posZ += glm::linearRand(-40.0f, 40.0f);
        scene.objects.push_back(std::move(obj));
        time = 0;
        counter += 1;
    }

    return true;
}

void Generator::render(Scene &scene) {
    // Generator will not be rendered
}
