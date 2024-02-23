#pragma once
#include "BaseObject.h"
#include <memory>

class GraphicsObject; // Forward declaration

class IAnimation : public BaseObject
{
protected:
	std::shared_ptr<GraphicsObject> object;

public:
	IAnimation() : object(nullptr) {}
	virtual ~IAnimation() = default;

	void SetObject(std::shared_ptr<GraphicsObject> object);
	virtual void Update(double elapsedSeconds) = 0;
};

