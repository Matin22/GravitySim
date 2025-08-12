#include "camera.hpp"
#include <iostream>

Camera::Camera(glm::vec3 inPosition, glm::vec3 inUp, float inYaw, float inPitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSens(SENS), zoom(ZOOM)
{
    position = inPosition;
    worldUp = inUp;
    yaw = inYaw;
    pitch = inPitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float inYaw, float inPitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSens(SENS), zoom(ZOOM)
{
    position = glm::vec3(posX, posY, posZ);
    worldUp = glm::vec3(upX, upY, upZ);
    yaw = inYaw;
    pitch = inPitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)   position += front * velocity;
    if (direction == BACKWARD)  position -= front * velocity;
    if (direction == LEFT)      position -= right * velocity;
    if (direction == RIGHT)     position += right * velocity;
}

void Camera::ProcessMouseMovement(float offsetX, float offsetY, GLboolean constrainPitch)
{
    offsetX *= mouseSens;
    offsetY *= mouseSens;

    yaw     += offsetX;
    pitch   += offsetY;

    if (constrainPitch)
    {
        if (pitch > 89.0f)  pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float offsetY)
{
    zoom -= static_cast<float>(offsetY);
    if (zoom < 1.0f) zoom = 1.0;
    if (zoom > 45.0f) zoom = 45.0;
}

void Camera::updateCameraVectors()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);

    right   = glm::normalize(glm::cross(front, worldUp));
    up      = glm::normalize(glm::cross(right, front));
}