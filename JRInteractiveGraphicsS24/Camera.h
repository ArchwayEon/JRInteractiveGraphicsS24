#pragma once
#include "BaseObject.h"
#include <glm/glm.hpp>

class Camera : public BaseObject
{
private:
	glm::mat4 lookFrame;
	glm::mat4 referenceFrame;
	float moveSpeed = 10.0f;

public:
	Camera() : lookFrame(1.0f), referenceFrame(1.0f) {}

	void SetLookFrame(glm::mat4 lookFrame) { this->lookFrame = lookFrame; }
	void SetMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
	void SetPosition(glm::vec3 position);
	glm::vec3 GetPosition() const;
	glm::mat4 LookForward() const;
	glm::mat4 LookAt(glm::vec3 target) const;
	void MoveForward(double elapsedSeconds);
	void MoveBackward(double elapsedSeconds);
	void MoveLeft(double elapsedSeconds);
	void MoveRight(double elapsedSeconds);
	void MoveUp(double elapsedSeconds);
	void MoveDown(double elapsedSeconds);
};

