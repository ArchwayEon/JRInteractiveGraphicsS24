#include "IGraphicsWorld.h"
#include "GraphicsEnvironment.h"
#include "ObjectManager.h"

IGraphicsWorld::IGraphicsWorld(std::shared_ptr<GraphicsEnvironment> env) :
	projection(1.0f), view(1.0f)
{
	_env = env;
	objectManager = std::make_shared<ObjectManager>();
}

void IGraphicsWorld::AddObject(
	const std::string& name, std::shared_ptr<GraphicsObject> object)
{
	_env->AddObject(name, object);
	objectManager->AddObject(name, object);
}


