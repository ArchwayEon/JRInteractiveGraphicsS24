#pragma once
#include "IAnimation.h"

enum BackForthState {
	MovingForward, MovingBackward
};

class BackForthAnimation :  public IAnimation
{
protected:
	BackForthState state = MovingForward;
	glm::vec3 direction;
	float speed;
	float distanceToMove = 0.0f;
	float distanceMoved = 0.0f;
public:
	BackForthAnimation(std::shared_ptr<GraphicsObject> object,
		glm::vec3 direction, float distanceToMove = 5.0f, float speed = 5.0f)
		: IAnimation(object), direction(direction),
		distanceToMove(distanceToMove), speed(speed) {}
	void Update(double elapsedSeconds);
};

