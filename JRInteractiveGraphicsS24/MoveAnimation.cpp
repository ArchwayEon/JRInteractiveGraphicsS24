#include "MoveAnimation.h"
#include "GraphicsObject.h"

void MoveAnimation::Update(double elapsedSeconds)
{
	if (object == nullptr) return;
	if (distanceMoved >= distanceToMove) {
		distanceMoved = 0;
		state = NOT_MOVING;
		FlipDirection();
		return;
	}
	if (state == MOVING) {
		float deltaSpeed = static_cast<float>(speed * elapsedSeconds);
		auto& frame = object->GetLocalReferenceFrame();
		glm::vec3 pos = frame.GetPosition();
		pos += direction * deltaSpeed;
		frame.SetPosition(pos);
		distanceMoved += deltaSpeed;
	}
}
