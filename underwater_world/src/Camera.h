#ifndef UNDERWATER_WORLD_CAMERA_H
#define UNDERWATER_WORLD_CAMERA_H
#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

class Camera {
public:
    // Camera position
    bool firstFollow = true;
    glm::vec3 position{0, 0, 0};
    glm::vec3 positionOffset{0, 0, 0};
    glm::vec3 offset{0, 2.5, 0};
    glm::vec3 rotation{0, 0, 0};
    glm::vec3 up{0, 1, 0};

    float distanceY = 2;
    float distanceZ = 5;
    glm::vec3 startPos;
    glm::vec3 destPos;
    glm::vec3 startPosLookAt;
    glm::vec3 destPosLookAt;
    float t;
    // Camera mode
    enum Mode {
        MOVING,
        ROTATING,
        FOLLOW
    } mode = FOLLOW;
    // Camera matrices

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;


    explicit Camera(float fow = 90.0f, float ratio = 1.9001f, float near = 0.0001f, float far = 10000.0f);

    void update();

    void moveTo(const glm::vec3 &pos, const glm::vec3 &rot);
};


#endif //UNDERWATER_WORLD_CAMERA_H
