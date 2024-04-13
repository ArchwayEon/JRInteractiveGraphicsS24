#include "RotateAnimation.h"
#include "GraphicsObject.h"
#include <glm/gtc/matrix_transform.hpp>

void RotateAnimation::Update(double elapsedSeconds)
{
	if (object == nullptr) return;
	float deltaSpeed = static_cast<float>(speed * elapsedSeconds);
	auto& referenceFrame = object->GetLocalReferenceFrame();
	referenceFrame.RotateAxis(axis, deltaSpeed);
}
