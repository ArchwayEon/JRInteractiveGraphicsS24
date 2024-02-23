#include "IAnimation.h"
#include "GraphicsObject.h"

void IAnimation::SetObject(std::shared_ptr<GraphicsObject> object)
{
	this->object = object;
}
