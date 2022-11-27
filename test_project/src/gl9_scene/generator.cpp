#include <glm/gtc/random.hpp>
#include <ppgso/ppgso.h>

#include "generator.h"
#include "Dolphin.h"
#include "Fish.h"

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
        obj->posY = -1;
        scene.objects.push_back(std::move(obj));
        auto fishs = std::make_unique<Fish>();
        fishs->position.x += glm::linearRand(-40.0f, 40.0f);
        fishs->position.y += glm::linearRand(-60.0f, 0.0f);
        fishs->position.z += glm::linearRand(-40.0f, 40.0f);
        fishs->posY = fishs->position.y ;
        fishs->posZ = fishs->position.z ;
        fishs->posX = fishs->position.x ;
        scene.objects.push_back(std::move(fishs));
        time = 0;
        counter += 1;
    }

    return true;
}

void Generator::render(Scene &scene) {
    // Generator will not be rendered
}
