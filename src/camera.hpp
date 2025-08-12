#ifndef CAMERA_H
#define CAMERA_H

#include <GLAD/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW     = -90.0f;
const float PITCH   = 0.0f;
const float SPEED   = 2.5f;
const float SENS    = 0.1f;
const float ZOOM    = 45.0f;

class Camera
{

public:
    // camera attribs
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // euler angle
    float yaw;
    float pitch;

    // options
    float movementSpeed;
    float mouseSens;
    float zoom;

    Camera(glm::vec3 inPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 inUp = glm::vec3(0.0f, 1.0f, 0.0f), float inYaw = YAW, float inPitch = PITCH);

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float inYaw, float inPitch);

    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float offsetX, float offsetY, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float offsetY);

private:
    void updateCameraVectors();
};

#endif