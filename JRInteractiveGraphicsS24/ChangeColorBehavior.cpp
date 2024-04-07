#include "ChangeColorBehavior.h"
#include "GraphicsObject.h"
#include "IGenerator.h"

void ChangeColorBehavior::StoreDefaults()
{
	if (object == nullptr) return;
	defaultColor = this->object->GetMaterial().color;
}

void ChangeColorBehavior::SetParameter(std::shared_ptr<IParams> params)
{
	this->params = std::static_pointer_cast<ChangeColorParams>(params);
}

void ChangeColorBehavior::Update(double elapsedSeconds)
{
	if (object == nullptr) return;
	if (object->IsOverlapping()) {
		object->GetMaterial().color = toColor;
	}
	else {
		object->GetMaterial().color = defaultColor;
	}
	object->GetGenerator()->Generate();
}
