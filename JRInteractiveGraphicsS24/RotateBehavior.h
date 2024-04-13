#pragma once
#include "IBehavior.h"
#include "GraphicsStructures.h"
#include <memory>
#include "ReferenceFrame.h"

class RotateBehavior : public IBehavior
{
protected:
	std::shared_ptr<RotateParams> params;
	ReferenceFrame defaultFrame;
public:
	RotateBehavior(std::shared_ptr<GraphicsObject> object);
	void StoreDefaults() override;
	void SetUpParameter() override;
	void SetParameter(std::shared_ptr<IParams> params) override;
	void Update(double elapsedSeconds) override;
	std::shared_ptr<RotateParams> GetParameter() {
		return params;
	}
};

