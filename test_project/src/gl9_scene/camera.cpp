#include <glm/glm.hpp>

#include "camera.h"


Camera::Camera(float fow, float ratio, float near, float far) {
  float fowInRad = (ppgso::PI/180.0f) * fow;

    projectionMatrix = glm::perspective(fowInRad, ratio, near, far);

    position = positionOffset = {0, distanceY, -distanceZ};
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

  viewMatrix = lookAt(position, position-back, up);
}
void Camera::moveTo(const glm::vec3 &pos, const glm::vec3 &rot) {
    if (mode == FOLLOW) {
        glm::vec3 deltaRot;
        deltaRot.y = (distanceZ * sin(rotation.y * -1)) - (distanceZ * sin(rot.y * -1));
        deltaRot.z = (distanceZ * -cos(rotation.y * -1)) - (distanceZ * -cos(rot.y * -1));

        rotation = rot;
        position -= deltaRot;
    }
    else if (mode == STATIONARY) {
        rotation = rot;
        position = pos;
    }
    else if (mode == MOVING) {
        t = 0;
        startPos = position;
        destPos = pos;
    }
}
glm::vec3 Camera::cast(double u, double v) {
  // Create point in Screen coordinates
  glm::vec4 screenPosition{u,v,0.0f,1.0f};

  // Use inverse matrices to get the point in world coordinates
  auto invProjection = glm::inverse(projectionMatrix);
  auto invView = glm::inverse(viewMatrix);

  // Compute position on the camera plane
  auto planePosition = invView * invProjection * screenPosition;
  planePosition /= planePosition.w;

  // Create direction vector
  auto direction = glm::normalize(planePosition - glm::vec4{position,1.0f});
  return glm::vec3{direction};
}
