#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

void Camera::SetPosition(glm::vec3 position)
{
	referenceFrame[3] = glm::vec4(position, 1.0f);
}

glm::mat4 Camera::LookForward() const
{
    glm::vec3 cameraPosition = referenceFrame[3];
    glm::vec3 cameraForward = -lookFrame[2];
    glm::vec3 cameraTarget = cameraPosition + cameraForward;
    glm::vec3 cameraUp = lookFrame[1];
	return glm::lookAt(cameraPosition, cameraTarget, cameraUp);
}

glm::mat4 Camera::LookAt(glm::vec3 target) const
{
    glm::vec3 cameraPosition = referenceFrame[3];
    glm::vec3 cameraUp = lookFrame[1];
    return glm::lookAt(cameraPosition, target, cameraUp);
}

void Camera::MoveBackward(double elapsedSeconds)
{
    glm::vec3 backward = lookFrame[2];
    glm::vec3 position = referenceFrame[3];
    backward = backward * static_cast<float>(moveSpeed * elapsedSeconds);
    position = position + backward;
    referenceFrame[3] = glm::vec4(position, 1.0f);
}

void Camera::MoveForward(double elapsedSeconds)
{
    glm::vec3 forward = -lookFrame[2];
    glm::vec3 position = referenceFrame[3];
    forward = forward * static_cast<float>(moveSpeed * elapsedSeconds);
    position = position + forward;
    referenceFrame[3] = glm::vec4(position, 1.0f);
}

void Camera::MoveLeft(double elapsedSeconds)
{
    glm::vec3 toLeft = -lookFrame[0];
    glm::vec3 position = referenceFrame[3];
    toLeft = toLeft * static_cast<float>(moveSpeed * elapsedSeconds);
    position = position + toLeft;
    referenceFrame[3] = glm::vec4(position, 1.0f);
}

void Camera::MoveRight(double elapsedSeconds)
{
    glm::vec3 toRight = lookFrame[0];
    glm::vec3 position = referenceFrame[3];
    toRight = toRight * static_cast<float>(moveSpeed * elapsedSeconds);
    position = position + toRight;
    referenceFrame[3] = glm::vec4(position, 1.0f);
}

void Camera::MoveUp(double elapsedSeconds)
{
    glm::vec3 up = referenceFrame[1];
    glm::vec3 position = referenceFrame[3];
    up = up * static_cast<float>(moveSpeed * elapsedSeconds);
    position = position + up;
    referenceFrame[3] = glm::vec4(position, 1.0f);
}

void Camera::MoveDown(double elapsedSeconds)
{
    glm::vec3 down = -referenceFrame[1];
    glm::vec3 position = referenceFrame[3];
    down = down * static_cast<float>(moveSpeed * elapsedSeconds);
    position = position + down;
    referenceFrame[3] = glm::vec4(position, 1.0f);
}


