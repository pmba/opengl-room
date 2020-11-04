#include "../camera.h"
#include "../glm/gtx/transform.hpp"

Camera::Camera() : 
    viewDirection(0.0f, 0.0f, -1.0f),
    UP(0.0f, 1.0f, 0.0f),
    CAMERA_YAW(0.005f),
    CAMERA_PITCH(0.005f),
    MOVEMENT_SPEED(0.1f)
{
}

void Camera::mouseUpdate(const glm::vec2& mousePosition) {
    // Calculate the mouseDelta, i.e the mouse x and y position variance
    glm::vec2 mouseDelta = mousePosition - oldMousePosition;

    // Prevent the camera to do a "hard" movement
    if (glm::length(mouseDelta) > 50.0f) {
        oldMousePosition = mousePosition;
        return;
    }

    // Calculate the horizontal viewDirection
    viewDirection = glm::mat3(glm::rotate(mouseDelta.x * CAMERA_YAW, UP)) * viewDirection;
    // Calculate the cross product between the viewDirection and the UP vector to rotate around it
    glm::vec3 vecToRotateAround = glm::cross(viewDirection, UP);
    // Calculate the vertical viewDirection around the previous [vecToRotateAround] vector
    viewDirection = glm::mat3(glm::rotate(mouseDelta.y * CAMERA_PITCH, vecToRotateAround)) * viewDirection;

    // Save the mousePosition
    oldMousePosition = mousePosition;
}

glm::mat3 Camera::getLookAtMatrix() const {
    // The view direction will always be one unit long
    return glm::mat3(position, position + viewDirection, UP);
}

/* Camera Movements Functions */

void Camera::moveForward() {
    // View direction its the direction of the movement and MOVEMENT_SPEED its the magnitude 
    position += viewDirection * MOVEMENT_SPEED;
}

void Camera::moveBackward() {
    // View direction its the direction of the movement and MOVEMENT_SPEED its the magnitude 
    position -= viewDirection * MOVEMENT_SPEED;
}

void Camera::moveLeft() {
    // To move left or right we need to calculate the cross product between the direction 
    // and the UP vector
    glm::vec3 moveDirection = glm::cross(viewDirection, UP);
    position -= moveDirection * MOVEMENT_SPEED;
}

void Camera::moveRight() {
     // To move left or right we need to calculate the cross product between the direction 
    // and the UP vector
    glm::vec3 moveDirection = glm::cross(viewDirection, UP);
    position += moveDirection * MOVEMENT_SPEED;
}
