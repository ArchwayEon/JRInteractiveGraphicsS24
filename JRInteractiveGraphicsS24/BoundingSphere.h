#pragma once
#include <glm/glm.hpp>

class BoundingSphere
{
protected:
	float radius = 1.0f;
	glm::vec3 position{};

public:
	BoundingSphere(float radius = 1.0f) {
		this->radius = radius;
	}
	~BoundingSphere() = default;

	void SetPosition(glm::vec3 position) {
		this->position = position;
	}
	bool OverlapsWith(const BoundingSphere& otherSphere) const;
};

