#pragma once
#include <memory>
#include "GraphicsStructures.h"

class GraphicsObject;

class IGenerator
{
protected:
	std::shared_ptr<GraphicsObject> graphicsObject = nullptr;

public:
	IGenerator() = default;
	virtual ~IGenerator() = default;

	void SetGraphicsObject(std::shared_ptr<GraphicsObject> graphicsObject) {
		this->graphicsObject = graphicsObject;
	}

	virtual void SetParameters(std::shared_ptr<IParams> params) = 0;
	virtual void Generate() = 0;
	virtual void SetUpDynamicBuffers() = 0;
};

