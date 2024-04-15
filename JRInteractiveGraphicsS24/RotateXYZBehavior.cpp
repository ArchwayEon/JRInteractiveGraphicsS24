#include "RotateXYZBehavior.h"
#include "GraphicsObject.h"
#include <glm/gtc/matrix_transform.hpp>

RotateXYZBehavior::RotateXYZBehavior(std::shared_ptr<GraphicsObject> object) :
	IBehavior(object), defaultFrame()
{
}

void RotateXYZBehavior::StoreDefaults()
{
	if (object == nullptr) return;
	defaultFrame = object->GetReferenceFrame();
	defaultFrame.SetPosition({ 0.0f, 0.0f, 0.0f });
}

void RotateXYZBehavior::SetUpParameters()
{
	this->params = std::make_shared<RotateXYZParams>();
}

void RotateXYZBehavior::SetParameter(std::shared_ptr<IParams> params)
{
	auto rp = std::static_pointer_cast<RotateXYZParams>(params);
	if (this->params != nullptr) {
		this->params->degreesX = rp->degreesX;
		this->params->degreesY = rp->degreesY;
		this->params->degreesZ = rp->degreesZ;
	}
	else {
		this->params = rp;
	}
}

void RotateXYZBehavior::Update(double elapsedSeconds)
{
	if (object == nullptr) return;
	if (params == nullptr) return;
	auto frame = defaultFrame;
	frame.RotateX(params->degreesX);
	frame.RotateY(params->degreesY);
	frame.RotateZ(params->degreesZ);
	frame.SetPosition(object->GetPosition());
	object->SetReferenceFrame(frame);
}
