#pragma once
#include <memory>
#include <vector>
#include "GraphicsObject.h"

class Scene
{
private:
	std::vector<std::shared_ptr<GraphicsObject>> objects;
	Light globalLight{};
	Light localLight{};

public:
	Scene();
	~Scene() = default;
	inline const std::vector<std::shared_ptr<GraphicsObject>>& GetObjects() const {
		return objects;
	}
	void AddObject(std::shared_ptr<GraphicsObject> object);
	void Update(float angle, float childAngle);

	Light& GetGlobalLight() { return globalLight; }
	Light& GetLocalLight() { return localLight; }
};

