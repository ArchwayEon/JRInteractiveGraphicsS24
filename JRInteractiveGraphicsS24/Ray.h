#pragma once
#include <glm/glm.hpp>
class Ray
{
protected:
	glm::vec3 startPoint{};
	glm::vec3 direction{};

public:
	void Set(glm::vec3 start, glm::vec3 end);

	void SetDirection(glm::vec3 direction) {
		this->direction = glm::normalize(direction);
	}
	const glm::vec3& GetDirection() const {
		return direction;
	}

	void SetStartPoint(glm::vec3 start) {
		startPoint = start;
	}
	const glm::vec3& GetStartPoint() const {
		return startPoint;
	}

	glm::vec3 GetPoint(float offset) const {
		return startPoint + (offset * direction);
	}
};

