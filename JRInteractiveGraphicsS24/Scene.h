#pragma once
#include <memory>
#include <vector>
#include "GraphicsObject.h"

class Scene
{
private:
	std::vector<std::shared_ptr<GraphicsObject>> objects;

public:
	Scene() = default;
	~Scene() = default;
	inline const std::vector<std::shared_ptr<GraphicsObject>>& GetObjects() const {
		return objects;
	}
	void AddObject(std::shared_ptr<GraphicsObject> object);
	void Update(float angle, float childAngle);

	const std::shared_ptr<GraphicsObject>& GetGraphicsObject(int index);
};

