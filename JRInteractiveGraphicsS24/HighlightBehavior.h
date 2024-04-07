#pragma once
#include "IBehavior.h"
#include "GraphicsStructures.h"
#include <memory>

class HighlightBehavior :  public IBehavior
{
protected:
	std::shared_ptr<HighlightParams> params;
	float ambientIntensity = 0.0;
public:
	void StoreDefaults() override;
	void SetParameter(std::shared_ptr<IParams> params) override;
	void Update(double elapsedSeconds) override;
};

