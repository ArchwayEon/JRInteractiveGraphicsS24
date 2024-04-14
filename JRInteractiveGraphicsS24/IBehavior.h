#pragma once
#include "GraphicsStructures.h"
#include <memory>

class GraphicsObject; // Forward declaration

class IBehavior
{
protected:
	std::shared_ptr<GraphicsObject> object;

public:
	IBehavior() : object(nullptr) {}
	IBehavior(std::shared_ptr<GraphicsObject> object) : object(object) {}
	virtual ~IBehavior() = default;

	virtual void SetObject(std::shared_ptr<GraphicsObject> object);
	virtual void Update(double elapsedSeconds) = 0;

	virtual void SetUpParameters() {};
	virtual void SetParameter(std::shared_ptr<IParams> params) {};
	virtual void StoreDefaults() {};
};

