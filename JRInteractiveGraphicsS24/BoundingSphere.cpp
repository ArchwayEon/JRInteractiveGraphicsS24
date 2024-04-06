#include "BoundingSphere.h"

bool BoundingSphere::OverlapsWith(const BoundingSphere& otherSphere) const
{
	auto distance = glm::distance(otherSphere.position, this->position);
	auto radiiSum = this->radius + otherSphere.radius;
	return distance < radiiSum;
}
