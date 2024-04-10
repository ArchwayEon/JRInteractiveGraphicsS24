#include "RotateBehavior.h"
#include "GraphicsObject.h"
#include <glm/gtc/matrix_transform.hpp>

RotateBehavior::RotateBehavior(std::shared_ptr<GraphicsObject> object) :
	IBehavior(object)
{
}

void RotateBehavior::StoreDefaults()
{
	if (object == nullptr) return;
	defaultFrame[0] = object->GetReferenceFrame()[0];
	defaultFrame[1] = object->GetReferenceFrame()[1];
	defaultFrame[2] = object->GetReferenceFrame()[2];
	//defaultFrame[3] = object->GetReferenceFrame()[3];
}

void RotateBehavior::SetParameter(std::shared_ptr<IParams> params)
{
	this->params = std::static_pointer_cast<RotateParams>(params);
}

void RotateBehavior::Update(double elapsedSeconds)
{
	if (object == nullptr) return;
	glm::mat4 frame(1.0f);
	frame =	glm::rotate(defaultFrame, 
		glm::radians(params->degrees), params->axis);
	frame[3] = object->GetReferenceFrame()[3];
	object->SetReferenceFrame(frame);
}
