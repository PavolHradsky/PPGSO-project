//
// Created by hrads on 5. 12. 2022.
//
#include <glm/gtc/random.hpp>
#include <ppgso/ppgso.h>

#include "bubbleGenerator.h"
#include "Bubble.h"
#include "shaders/texture_vert_glsl.h"

bool BubbleGenerator::update(Scene &scene, float dt) {
    // Accumulate time
    time1 += dt;
    // Add object to scene when time reaches certain level
    if (time1 > .5f) {
        auto obj = std::make_unique<Bubble>();
        obj->position.x = glm::linearRand(-35.0f, -05.0f);
        obj->position.z = glm::linearRand(-35.0f, -05.0f);
        obj->position.y = -70;
        obj->scale *= glm::linearRand(0.3, 2.0);
        scene.objects.push_back(std::move(obj));
        time1 = 0;
    }
    return true;
}

void BubbleGenerator::render(Scene &scene) {
    // Generator will not be rendered
}
