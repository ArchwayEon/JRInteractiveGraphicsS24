#pragma once
#include "IBehavior.h"
#include "GraphicsStructures.h"
#include <memory>
#include "ReferenceFrame.h"

class RotateXYZBehavior :  public IBehavior
{
protected:
	std::shared_ptr<RotateXYZParams> params;
	ReferenceFrame defaultFrame;
public:
	RotateXYZBehavior(std::shared_ptr<GraphicsObject> object);
	void StoreDefaults() override;
	void SetUpParameters() override;
	void SetParameter(std::shared_ptr<IParams> params) override;
	void Update(double elapsedSeconds) override;
	std::shared_ptr<RotateXYZParams> GetParameter() {
		return params;
	}
};

