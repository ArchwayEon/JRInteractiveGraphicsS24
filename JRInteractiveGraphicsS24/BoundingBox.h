#pragma once
#include <glm/glm.hpp>
#include "GeometricPlane.h"
#include <vector>

class BoundingBox
{
public:
	static const int FRONT = 0;
	static const int BACK = 1;
	static const int LEFT = 2;
	static const int RIGHT = 3;
	static const int TOP = 4;
	static const int BOTTOM = 5;

protected:
	float width = 1.0f, height = 1.0f, depth = 1.0f;
	glm::mat4 frame;
	glm::mat4 invFrame;
	GeometricPlane planes[6];
	std::vector<float> intersections;
	glm::vec3 intersectionPoint{};

public:
	void SetReferenceFrame(glm::mat4 frameIn) {
		frame = frameIn;
		invFrame = glm::inverse(frame);
	}
	void Create(float width, float height, float);
};

