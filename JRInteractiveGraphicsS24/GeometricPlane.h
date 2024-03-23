#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
class GeometricPlane
{
protected:
	glm::vec3 normal = { 0.0f, 1.0f, 0.0f };
	float distanceFromOrigin = 0.0f;

public:
	void Set(glm::vec3 normal, float distanceFromOrigin);

	float GetIntersectionOffset(const Ray& ray) const;
};

