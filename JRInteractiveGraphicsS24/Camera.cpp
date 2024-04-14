#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

void Camera::SetPosition(glm::vec3 position)
{
    referenceFrame.SetPosition(position);
}

glm::mat4 Camera::GetLookForwardViewMatrix() const
{
    glm::vec3 cameraPosition = referenceFrame.GetPosition();
    glm::vec3 cameraForward = -lookFrame.GetZAxis();
    glm::vec3 cameraTarget = cameraPosition + cameraForward;
    glm::vec3 cameraUp = lookFrame.GetYAxis();
	return glm::lookAt(cameraPosition, cameraTarget, cameraUp);
}

glm::vec3 Camera::GetPosition() const
{
    return referenceFrame.GetPosition();
}

glm::mat4 Camera::GetLookAtViewMatrix(glm::vec3 target) const
{
    glm::vec3 cameraPosition = referenceFrame.GetPosition();
    glm::vec3 cameraUp = lookFrame.GetYAxis();
    return glm::lookAt(cameraPosition, target, cameraUp);
}

void Camera::MoveBackward(double elapsedSeconds)
{
    glm::vec3 backward = lookFrame.GetZAxis();
    glm::vec3 position = referenceFrame.GetPosition();
    backward = backward * static_cast<float>(moveSpeed * elapsedSeconds);
    position = position + backward;
    referenceFrame.SetPosition(position);
}

void Camera::MoveForward(double elapsedSeconds)
{
    glm::vec3 forward = -lookFrame.GetZAxis();
    glm::vec3 position = referenceFrame.GetPosition();
    forward = forward * static_cast<float>(moveSpeed * elapsedSeconds);
    position = position + forward;
    referenceFrame.SetPosition(position);
}

void Camera::MoveLeft(double elapsedSeconds)
{
    glm::vec3 toLeft = -lookFrame.GetXAxis();
    glm::vec3 position = referenceFrame.GetPosition();
    toLeft = toLeft * static_cast<float>(moveSpeed * elapsedSeconds);
    position = position + toLeft;
    referenceFrame.SetPosition(position);
}

void Camera::MoveRight(double elapsedSeconds)
{
    glm::vec3 toRight = lookFrame.GetXAxis();
    glm::vec3 position = referenceFrame.GetPosition();
    toRight = toRight * static_cast<float>(moveSpeed * elapsedSeconds);
    position = position + toRight;
    referenceFrame.SetPosition(position);
}

void Camera::MoveUp(double elapsedSeconds)
{
    glm::vec3 up = lookFrame.GetYAxis();
    glm::vec3 position = referenceFrame.GetPosition();
    up = up * static_cast<float>(moveSpeed * elapsedSeconds);
    position = position + up;
    referenceFrame.SetPosition(position);
}

void Camera::MoveDown(double elapsedSeconds)
{
    glm::vec3 down = -lookFrame.GetYAxis();
    glm::vec3 position = referenceFrame.GetPosition();
    down = down * static_cast<float>(moveSpeed * elapsedSeconds);
    position = position + down;
    referenceFrame.SetPosition(position);
}


