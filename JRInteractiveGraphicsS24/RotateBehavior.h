#pragma once
#include "IBehavior.h"
#include "GraphicsStructures.h"
#include <memory>

class RotateBehavior : public IBehavior
{
protected:
	std::shared_ptr<RotateParams> params;
	glm::mat4 defaultFrame = glm::mat4(1.0f);
public:
	RotateBehavior(std::shared_ptr<GraphicsObject> object);
	void StoreDefaults() override;
	void SetParameter(std::shared_ptr<IParams> params) override;
	void Update(double elapsedSeconds) override;
};

