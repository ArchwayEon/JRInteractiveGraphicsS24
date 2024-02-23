#include "ObjectManager.h"

void ObjectManager::AddObject(const std::string& name, std::shared_ptr<GraphicsObject> object)
{
    if (objectMap.contains(name)) return;
    objectMap[name] = object;
}

std::shared_ptr<GraphicsObject> ObjectManager::GetObject(const std::string& name)
{
    if (objectMap.contains(name) == false) return nullptr;
    return objectMap[name];
}

void ObjectManager::Update(double elapsedSeconds)
{
    for (auto& [name, object] : objectMap) {
        object->Update(elapsedSeconds);
    }
}
