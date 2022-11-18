#include <glm/glm.hpp>

#include "camera.h"


Camera::Camera(float fow, float ratio, float near, float far) {
    float fowInRad = (ppgso::PI / 180.0f) * fow;

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
    back = rotate(rotation.x, rotation.y);
    viewMatrix = lookAt(position, position - back, up);
}

// mouseMove
void Camera::mouseMove(int x, int y) {

    rotation.x += (x - position.x) * 0.01f;
    rotation.y += (y - position.y) * 0.01f;
    if (rotation.y > 1.5f) rotation.y = 1.5f;
    if (rotation.y < -1.5f) rotation.y = -1.5f;
    position.x = x;
    position.y = y;
}

void Camera::moveTo(const glm::vec3 &pos, const glm::vec3 &rot) {
    if (mode == FOLLOW) {
        glm::vec3 deltaRot;
        deltaRot.y = (distanceZ * sin(rotation.y * -1)) - (distanceZ * sin(rot.y * -1));
        deltaRot.z = (distanceZ * -cos(rotation.y * -1)) - (distanceZ * -cos(rot.y * -1));

        rotation = rot;
        position -= deltaRot;
    } else if (mode == STATIONARY) {
        rotation = rot;
        position = pos;
    } else if (mode == MOVING) {
        t = 0;
        startPos = position;
        destPos = pos;
    }
}

glm::vec3 Camera::cast(double u, double v) {
    // Create point in Screen coordinates
    glm::vec4 screenPosition{u, v, 0.0f, 1.0f};

    // Use inverse matrices to get the point in world coordinates
    auto invProjection = glm::inverse(projectionMatrix);
    auto invView = glm::inverse(viewMatrix);

    // Compute position on the camera plane
    auto planePosition = invView * invProjection * screenPosition;
    planePosition /= planePosition.w;

    // Create direction vector
    auto direction = glm::normalize(planePosition - glm::vec4{position, 1.0f});
    return glm::vec3{direction};
}

glm::vec3 Camera::rotate(double fi, double theta) {
    glm::vec3 result;
    result.x = position.x * cos(fi) - position.z * sin(fi);
    result.z = position.x * sin(fi) + position.z * cos(fi);
    result.y = position.y * cos(theta) - result.z * sin(theta);
    result.z = position.y * sin(theta) + result.z * cos(theta);
    return result;
}
