//
// Created by hrads on 5. 12. 2022.
//
#include <glm/gtc/random.hpp>
#include <ppgso/ppgso.h>

#include "bubbleGenerator.h"
#include "Bubble.h"
#include "shaders/texture_vert_glsl.h"
#include "shaders/texture_frag_glsl.h"
#include "shaders/my_texture_frag_glsl.h"

int counter1 = 0;
//auto shaderDark = std::make_unique<ppgso::Shader>(texture_vert_glsl, my_texture_frag_glsl);

bool BubbleGenerator::update(Scene &scene, float dt) {
    // Accumulate time
    time1 += dt;
    // Add object to scene when time reaches certain level
    if (time1 > 2) {
        auto obj = std::make_unique<Bubble>();
        obj->position.x = glm::linearRand(-35.0f, -05.0f);
        obj->position.z = glm::linearRand(-35.0f, -05.0f);
        obj->position.y = -70;
        scene.objects.push_back(std::move(obj));
        time1 = 0;
    }

    return true;
}

void BubbleGenerator::render(Scene &scene) {
    // Generator will not be rendered
}
