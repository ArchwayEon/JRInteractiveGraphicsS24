#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(float radius, glm::vec3 position)
{
	this->radius = radius;
	this->position = position;
}

BoundingSphere::BoundingSphere(
	const BoundingSphere& sphere1, const BoundingSphere& sphere2)
{
    glm::vec3 centerOffset = sphere2.position - sphere1.position;
    float distance = glm::length(centerOffset);
    float distanceSquared = distance * distance;
    float radiusDiff = sphere2.radius - sphere1.radius;

    // Check if the larger sphere encloses the small one
    if (radiusDiff * radiusDiff >= distanceSquared) {
        if (sphere1.radius > sphere2.radius) {
            position = sphere1.position;
            radius = sphere1.radius;
        }
        else {
            position = sphere2.position;
            radius = sphere2.radius;
        }
    }
    // Otherwise we need to work with partially overlapping spheres
    else {
        radius = (distance + sphere1.radius + sphere2.radius) * (0.5f);

        // The new center is based on one's center, moved towards two's center by
        // an amount proportional to the spheres' radii.
        position = sphere1.position;
        if (distance > 0) {
            position += centerOffset * ((radius - sphere1.radius) / distance);
        }
    }
}

BoundingSphere::BoundingSphere(const BoundingSphere& sphere)
{
    radius = sphere.radius;
    position = sphere.position;
}

bool BoundingSphere::OverlapsWith(const BoundingSphere& otherSphere) const
{
	auto distance = glm::distance(otherSphere.position, this->position);
	auto radiiSum = this->radius + otherSphere.radius;
	return distance < radiiSum;
}

float BoundingSphere::GetOverlapAmount(const BoundingSphere& otherSphere) const
{
    float overlap = 0;
    float distance = glm::length(position - otherSphere.position);
    float radiiSum = radius + otherSphere.radius;
    if (distance < radiiSum) {
        overlap = radiiSum - distance;
    }
    return overlap;
}

float BoundingSphere::GetGrowth(const BoundingSphere& other) const
{
    BoundingSphere newSphere(*this, other);
    // We return a value proportional to the change in surface area of the 
    // sphere.
    return (newSphere.radius * newSphere.radius) - (radius * radius);
}
