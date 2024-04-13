#include "RotateBehavior.h"
#include "GraphicsObject.h"
#include <glm/gtc/matrix_transform.hpp>

RotateBehavior::RotateBehavior(std::shared_ptr<GraphicsObject> object) :
	IBehavior(object), defaultFrame()
{
}

void RotateBehavior::StoreDefaults()
{
	if (object == nullptr) return;
	defaultFrame = object->GetReferenceFrame();
	defaultFrame.SetPosition({ 0.0f, 0.0f, 0.0f });
}

void RotateBehavior::SetUpParameter()
{
	this->params = std::make_shared<RotateParams>();
}

void RotateBehavior::SetParameter(std::shared_ptr<IParams> params)
{
	auto rp = std::static_pointer_cast<RotateParams>(params);
	if (this->params != nullptr) {
		this->params->axis = rp->axis;
		this->params->degrees = rp->degrees;
	}
	else {
		this->params = rp;
	}
}

void RotateBehavior::Update(double elapsedSeconds)
{
	if (object == nullptr) return;
	if (params == nullptr) return;
	defaultFrame.RotateAxis(params->axis, params->degrees);
	defaultFrame.SetPosition(object->GetPosition());
	object->SetReferenceFrame(defaultFrame);
}
