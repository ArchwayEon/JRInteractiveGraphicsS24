#include "Scene.h"

void Scene::AddObject(std::shared_ptr<GraphicsObject> object)
{
	objects.push_back(object);
}

void Scene::Update(float angle, float childAngle)
{
	for (auto& object : objects) {
		object->ResetOrientation();
		object->RotateLocalZ(angle);
		for (auto& child : object->GetChildren()) {
			child->ResetOrientation();
			child->RotateLocalZ(childAngle);
		}
	}
}

