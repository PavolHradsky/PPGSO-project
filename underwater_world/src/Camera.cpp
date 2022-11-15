#include <cmath>
#include <glm/glm.hpp>
#include "Camera.h"


Camera::Camera(float fow, float ratio, float near, float far) {
    float fowInRad = (ppgso::PI/180.0f) * fow;

    projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
    position = positionOffset = {0, distanceY, -distanceY};
}

void Camera::update() {
    if (mode == MOVING) {
        if (t < 900) {
            t++;
            auto result = glm::lerp(startPos, destPos, t / 900.0f);
            position.x = result.x;
            position.y = result.y;
            position.z = result.z;
        }
    }
    viewMatrix = glm::lookAt(position, position + offset, up);
}

void Camera::moveTo(const glm::vec3 &pos, const glm::vec3 &rot) {

    if (mode == FOLLOW) {
        if (firstFollow) {
            firstFollow = false;
            positionOffset = {0, distanceY, -distanceZ};
            rotation = {0, 0, 0};
        }

        glm::vec3 deltaRot;
        deltaRot.x = (float) ((distanceZ * std::sin(rotation.y * -1)) - (distanceZ * std::sin(rot.y * -1)));
        deltaRot.z = (float) ((distanceZ * -std::cos(rotation.y * -1)) - (distanceZ * -std::cos(rot.y * -1)));

        rotation = rot;

        position -= deltaRot;
        position -= destPos;
    } else {
        firstFollow = true;
    }
}