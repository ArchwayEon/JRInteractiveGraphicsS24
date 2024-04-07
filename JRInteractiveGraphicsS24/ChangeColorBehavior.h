#pragma once
#include "IBehavior.h"
#include "GraphicsStructures.h"
#include <memory>
#include <glm/glm.hpp>

class ChangeColorBehavior :  public IBehavior
{
protected:
	std::shared_ptr<ChangeColorParams> params;
	glm::vec3 toColor;
	glm::vec3 defaultColor = { 1.0f, 1.0f, 1.0f };

public:
	ChangeColorBehavior(glm::vec3 toColor = { 0.0f, 1.0f, 1.0f }) {
		this->toColor = toColor;
	}
	void StoreDefaults() override;
	void SetParameter(std::shared_ptr<IParams> params) override;
	void Update(double elapsedSeconds) override;
};

