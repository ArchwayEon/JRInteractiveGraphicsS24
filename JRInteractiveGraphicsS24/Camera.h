#pragma once
#include <glm/glm.hpp>
#include "ReferenceFrame.h"

class Camera
{
private:
	ReferenceFrame lookFrame;
	ReferenceFrame referenceFrame;
	float moveSpeed = 10.0f;

public:
	Camera() : lookFrame(), referenceFrame() {}

	void SetLookFrame(ReferenceFrame lookFrame) { this->lookFrame = lookFrame; }
	void SetMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
	void SetPosition(glm::vec3 position);
	glm::vec3 GetPosition() const;
	glm::mat4 GetLookForwardViewMatrix() const;
	glm::mat4 GetLookAtViewMatrix(glm::vec3 target) const;
	void MoveForward(double elapsedSeconds);
	void MoveBackward(double elapsedSeconds);
	void MoveLeft(double elapsedSeconds);
	void MoveRight(double elapsedSeconds);
	void MoveUp(double elapsedSeconds);
	void MoveDown(double elapsedSeconds);

	ReferenceFrame GetLookFrame() const { return lookFrame; }
	ReferenceFrame GetReferenceFrame() const { return referenceFrame; }

	//glm::vec3 TransformVectorToLocal(glm::vec3 v);
};

