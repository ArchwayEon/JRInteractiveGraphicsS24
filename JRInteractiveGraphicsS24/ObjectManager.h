#pragma once
#include <string>
#include <unordered_map>
#include "GraphicsObject.h"

class ObjectManager
{
private:
    std::unordered_map<std::string, std::shared_ptr<GraphicsObject>> objectMap;

public:
    ObjectManager() = default;
    ~ObjectManager() = default;

    void AddObject(const std::string& name, std::shared_ptr<GraphicsObject> object);
    std::shared_ptr<GraphicsObject> GetObject(const std::string& name);
    void Update(double elapsedSeconds);
    void SetBehaviorDefaults();
};

