#include "GeometricPlane.h"

void GeometricPlane::Set(glm::vec3 normal, float distanceFromOrigin)
{
	this->normal = glm::normalize(normal);
	this->distanceFromOrigin = fabs(distanceFromOrigin);
}

float GeometricPlane::GetIntersectionOffset(const Ray& ray) const
{
	float offset = -1.0f;
	// Project the normal onto the ray
	float nRayProj = glm::dot(normal, ray.GetDirection());
	// Check if the ray is parallel to the plane
	if (nRayProj == 0.0f) return offset;
	float nStartProj = glm::dot(normal, ray.GetStartPoint());
	offset = -(nStartProj + distanceFromOrigin) / nRayProj;
	// A negative value means the intersection offset is behind the start
	// point.
	return offset;
}
