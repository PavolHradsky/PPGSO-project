#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

/*!
 * Simple camera object that keeps track of viewMatrix and projectionMatrix
 * the projectionMatrix is by default constructed as perspective projection
 * the viewMatrix is generated from up, position and back vectors on update
 */
class Camera {
public:
    enum MODE { FOLLOW, STATIONARY, MOVING } mode = FOLLOW;
  glm::vec3 up{0,1,0};
  glm::vec3 position{0,0,0};
  glm::vec3 positionOffset{0, 0, 0};
  glm::vec3 back{0,0,-1};
    glm::vec3 offset{0, 2.5, 0};
  glm::vec3 forward{0,0,1};
    float t;
    glm::vec3 startPos;
    glm::vec3 destPos;
  glm::vec3 rotation{0, 0, 0};
  float distanceY = 2;
  float distanceZ = 5;
  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;
  float moveSpeed = 0.02f;
  float rotateSpeed = 0.005f;

  bool moveW, moveS, moveA, moveD, moveQ, moveE, rotateUp, rotateDown, rotateLeft, rotateRight = false;

  /*!
   * Create new Camera that will generate viewMatrix and projectionMatrix based on its position, up and back vectors
   * @param fow - Field of view in degrees
   * @param ratio - Viewport screen ratio (usually width/height of the render window)
   * @param near - Distance to the near frustum plane
   * @param far - Distance to the far frustum plane
   */
  Camera(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);

  /*!
   * Update Camera viewMatrix based on up, position and back vectors
   */
  void update();
    void moveTo(const glm::vec3 &pos, const glm::vec3 &rot);
  /*!
   * Get direction vector in world coordinates through camera projection plane
   * @param u - camera projection plane horizontal coordinate [-1,1]
   * @param v - camera projection plane vertical coordinate [-1,1]
   * @return Normalized vector from camera position to position on the camera projection plane
   */
  glm::vec3 cast(double u, double v);
  glm::vec3 rotate(double pitch, double yaw);

    void mouseMove(int x, int y);
};

