#include <glm/glm.hpp>
#include "camera.h"
#include "scene.h"

Camera::Camera(float fow, float ratio, float near, float far) {
    float fowInRad = (ppgso::PI / 180.0f) * fow;

    projectionMatrix = glm::perspective(fowInRad, ratio, near, far);

    position = positionOffset = {0, distanceY, -distanceZ};
}

void Camera::update() {
    if (animate) {
        if (t < 900) {
            t++;
            auto moving = glm::lerp(starting_position, ending_position, t / 900.0f);
            auto rotating = glm::lerp(starting_rotation, ending_rotation, t / 900.0f);
            position.x = moving.x;
            position.y = moving.y;
            position.z = moving.z;
            rotation.x = rotating.x;
            rotation.y = rotating.y;
            rotation.z = rotating.z;
        }
    }

    back = rotate(rotation.x, rotation.y);
    back = glm::normalize(back);
    viewMatrix = lookAt(position, position - back, up);
}

void Camera::moveTo(const glm::vec3 &start_pos, const glm::vec3 &end_pos, const glm::vec3 &start_rot,
                    const glm::vec3 &end_rot) {
    starting_position = start_pos;
    ending_position = end_pos;
    starting_rotation = start_rot;
    ending_rotation = end_rot;
}

glm::vec3 Camera::cast(double u, double v) const {
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

glm::vec3 Camera::rotate(double pitch, double yaw) {
    glm::vec3 direction;
    direction.x = cos(yaw) * cos(pitch);
    direction.y = sin(pitch);
    direction.z = sin(yaw) * cos(pitch);
    return glm::normalize(direction);
}
