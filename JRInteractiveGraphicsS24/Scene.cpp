#include "Scene.h"

Scene::Scene()
{
	globalLight.attenuationCoef = 0;
	globalLight.color = { 1.0f, 1.0f, 1.0f };
	globalLight.intensity = 0.5f;
	globalLight.position = glm::vec3(100.0f, 100.0f, 0.0f);
	localLight.attenuationCoef = 0;
	localLight.color = { 1.0f, 1.0f, 1.0f };
	localLight.intensity = 0.1f;
	localLight.position = glm::vec3(0.0f, 5.0f, 5.0f);
}

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

