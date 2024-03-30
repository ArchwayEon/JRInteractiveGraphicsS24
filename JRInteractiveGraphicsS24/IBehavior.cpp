#include "IBehavior.h"
#include "GraphicsObject.h"

void IBehavior::SetObject(std::shared_ptr<GraphicsObject> object)
{
	this->object = object;
}
