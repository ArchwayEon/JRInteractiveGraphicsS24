#pragma once
#include <memory>
#include "IBehavior.h"

class GraphicsObject; // Forward declaration

class IAnimation : public IBehavior
{
protected:

public:
	IAnimation() : IBehavior() {}
	virtual ~IAnimation() = default;
};

