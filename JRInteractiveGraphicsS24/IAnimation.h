#pragma once
#include <memory>
#include "IBehavior.h"

class GraphicsObject; // Forward declaration

class IAnimation : public IBehavior
{
protected:

public:
	IAnimation() : IBehavior() {}
	IAnimation(std::shared_ptr<GraphicsObject> object) : IBehavior(object) {}
	virtual ~IAnimation() = default;
};

