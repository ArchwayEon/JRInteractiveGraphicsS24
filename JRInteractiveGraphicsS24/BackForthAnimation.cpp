#include "BackForthAnimation.h"
#include "GraphicsObject.h"

void BackForthAnimation::Update(double elapsedSeconds)
{
	if (object == nullptr) return;
	float deltaSpeed = static_cast<float>(speed * elapsedSeconds);
	auto referenceFrame = object->GetReferenceFrame();
	auto position = referenceFrame.GetPosition();
	glm::vec3 newPos{};
	float deltaDistance = 0;
	switch (state) {
	case MovingForward:
		deltaDistance = glm::length(direction * deltaSpeed);
		if (distanceMoved + deltaDistance >= distanceToMove) {
			deltaDistance = distanceToMove - distanceMoved;
			state = MovingBackward;
			distanceMoved = 0;
		}
		else {
			distanceMoved += deltaDistance;
		}
		newPos = position + (direction * deltaDistance);
		break;
	case MovingBackward:
		deltaDistance = glm::length(-direction * deltaSpeed);
		if (distanceMoved + deltaDistance >= distanceToMove) {
			deltaDistance = distanceToMove - distanceMoved;
			state = MovingForward;
			distanceMoved = 0;
		}
		else {
			distanceMoved += deltaDistance;
		}
		newPos = position + (-direction * deltaDistance);
		break;
	}
	
	object->SetPosition(newPos);
}
