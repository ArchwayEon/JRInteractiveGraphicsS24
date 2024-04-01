#pragma once
#include "IAnimation.h"
class MoveAnimation :  public IAnimation
{
public:
	static const int NOT_MOVING = 0;
	static const int MOVING = 1;
private:
	glm::vec3 direction = { 0.0f, 0.0f, -1.0f };
	float speed;
	float distanceToMove;
	float distanceMoved = 0.0f;
	int state = NOT_MOVING;

public:
	MoveAnimation(float speed = 5.0f, float distanceToMove = 20.0f)
		: IAnimation(), speed(speed), distanceToMove(distanceToMove) {}
	void Update(double elapsedSeconds);
	int GetState() const { return state; }
	void SetState(int state) { this->state = state; }
	void FlipDirection() { direction = -direction; }
};

