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
        // rotate object to make him in right
        // make him in right position


        obj->rotation.x = 3 * ppgso::PI / 2;
        obj->rotation.y = 0;
        obj->rotation.z = 2 * ppgso::PI / 3;
        obj->position = position;
        obj->position.x += glm::linearRand(-10.0f, 10.0f) * 5;
        obj->position.y += 0;
        obj->position.z += glm::linearRand(-30.0f, 30.0f) * 10;
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
