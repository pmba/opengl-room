#pragma once
#include "glm/glm.hpp"

class Camera
{
    glm::vec3 position;
    glm::vec3 viewDirection;
    glm::vec2 oldMousePosition;

    // Camera Constants
    const glm::vec3 UP;

    const float CAMERA_YAW;
    const float CAMERA_PITCH;
    const float MOVEMENT_SPEED;

    public:
    Camera();

    /* User Input Functions */
    void mouseUpdate(const glm::vec2& mousePosition);

    /* Math Functions */
    glm::mat3 getLookAtMatrix() const;
    
    /* Camera Movements Functions */
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
};