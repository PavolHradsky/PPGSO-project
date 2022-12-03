#include <glm/gtc/random.hpp>
#include <ppgso/ppgso.h>

#include "generator.h"
#include "Dolphin.h"
#include "Fish.h"
#include "Seaweed.h"
#include "FishTail.h"
#include "shaders/texture_vert_glsl.h"
#include "shaders/texture_frag_glsl.h"
#include "shaders/my_texture_frag_glsl.h"

int counter = 0;
//auto shaderDark = std::make_unique<ppgso::Shader>(texture_vert_glsl, my_texture_frag_glsl);

bool Generator::update(Scene &scene, float dt) {
    // Accumulate time
    time += dt;

    // Add object to scene when time reaches certain level
    if (time > .3 and counter < 30) {
        auto obj = std::make_unique<Dolphin>();
        obj->speed = glm::linearRand(2.0f, 5.0f);
        obj->position.x += glm::linearRand(-120.0f, 120.0f);
        obj->position.z += glm::linearRand(-120.0f, 120.0f);
        obj->posZ = obj->position.z;
        obj->posX = obj->position.x;
        obj->posY = -1;
        scene.objects.push_back(std::move(obj));

        auto seaweeds = std::make_unique<Seaweed>();
        seaweeds->position.x += glm::linearRand(-120.0f, 120.0f);
        //TODO> interpolate this with the height of the sand via controlpoints
        seaweeds->position.y = -70;
        seaweeds->position.z += glm::linearRand(-120.0f, 120.0f);
        seaweeds->posY = seaweeds->position.y;
        seaweeds->posX =  seaweeds->position.x;
        seaweeds->posZ =  seaweeds->position.z;
        scene.objects.push_back(std::move(seaweeds));

        auto fishs = std::make_unique<Fish>();
        fishs->position.x += glm::linearRand(-120.0f, 120.0f);
        fishs->position.y += glm::linearRand(-70.0f, 0.0f);
        fishs->position.z += glm::linearRand(-120.0f, 120.0f);
        fishs->posY = fishs->position.y;
        fishs->posZ = fishs->position.z;
        fishs->posX = fishs->position.x;
//        fishs->shader = std::move(shaderDark);
        //scene.objects.push_back(std::move(fishs->fishTail));
        scene.objects.push_back(std::move(fishs));
        time = 0;
        counter += 1;
//        auto fishTail = std::make_unique<FishTail>();
//        fishTail->fish = std::move(fishs);
//        scene.objects.push_back(std::move(fishTail));
    }

    return true;
}

void Generator::render(Scene &scene) {
    // Generator will not be rendered
}
