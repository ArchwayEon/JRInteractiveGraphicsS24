#include "IGraphicsWorld.h"
#include "GraphicsEnvironment.h"

IGraphicsWorld::IGraphicsWorld(std::shared_ptr<GraphicsEnvironment> env) :
	projection(1.0f), view(1.0f)
{
	_env = env;
}

