#pragma once
#include <glm/glm.hpp>
#include <numbers>

class BoundingSphere
{
protected:
	float radius = 1.0f;
	glm::vec3 position{};

public:
	BoundingSphere(float radius = 1.0f) {
		this->radius = radius;
	}
	BoundingSphere(float radius, glm::vec3 position);
	// Creates a bounding sphere to enclose the two given bounding spheres.
	BoundingSphere(const BoundingSphere& sphere1, const BoundingSphere& sphere2);
	BoundingSphere(const BoundingSphere& sphere);
	~BoundingSphere() = default;
	float GetRadius() const { return radius; }
	void SetPosition(glm::vec3 position) {
		this->position = position;
	}
	bool OverlapsWith(const BoundingSphere& otherSphere) const;
	float GetOverlapAmount(const BoundingSphere& otherSphere) const;
	// Returns the volume of this bounding sphere. This is used
    // to calculate how to recurse into the bounding volume tree.
    // For a bounding sphere it is a simple calculation.
	float GetVolume() const{
		return (1.333333f) * (float)std::numbers::pi * radius * radius * radius;
	}
	// Reports how much this bounding sphere would have to grow by to 
    // incorporate the given bounding sphere. Note that this calculation returns
    // a value not in any particular units (i.e., its not a volume growth). In 
    // fact the best implementation takes into account the growth in surface 
    // area (after the Goldsmith-Salmon algorithm for tree construction).
	float GetGrowth(const BoundingSphere& other) const;
};

