#pragma once
#include "IAnimation.h"
#include <glm/glm.hpp>

class RotateAnimation : public IAnimation
{
private:
	glm::vec3 axis;
	float speed;

public:
	RotateAnimation(
		glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f), float speed = 90.0f)
		: IAnimation(), axis(axis), speed(speed) {}
	void Update(double elapsedSeconds);
};

