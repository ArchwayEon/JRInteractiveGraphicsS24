#pragma once
#include "IBehavior.h"
#include "GraphicsStructures.h"

class HighlightBehavior :  public IBehavior
{
protected:
	HighlightParams params;
	float ambientIntensity = 0.0;
public:
	void StoreDefaults() override;
	void SetParameter(IParams params) override;
	void Update(double elapsedSeconds) override;
};

