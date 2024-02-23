#include "RotateAnimation.h"
#include "GraphicsObject.h"
#include <glm/gtc/matrix_transform.hpp>

void RotateAnimation::Update(double elapsedSeconds)
{
	if (object == nullptr) return;
	float deltaSpeed = static_cast<float>(speed * elapsedSeconds);
	glm::mat4& referenceFrame = object->GetLocalReferenceFrame();
	referenceFrame = glm::rotate(referenceFrame, glm::radians(deltaSpeed), axis);
}
