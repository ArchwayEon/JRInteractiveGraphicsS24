#include "IGraphicsWorld.h"
#include "GraphicsEnvironment.h"
#include "ObjectManager.h"
#include "Shader.h"
#include "Renderer.h"

IGraphicsWorld::IGraphicsWorld(std::shared_ptr<GraphicsEnvironment> env) :
	projection(1.0f), view(1.0f)
{
	_env = env;
	objectManager = std::make_shared<ObjectManager>();
}

void IGraphicsWorld::AddObject(
	const std::string& name, std::shared_ptr<GraphicsObject> object)
{
	objectManager->AddObject(name, object);
}

void IGraphicsWorld::CreateRenderer(
	const std::string& name, std::shared_ptr<Shader> shader)
{
	auto renderer = std::make_shared<Renderer>(shader);
	rendererMap.emplace(name, renderer);
}

std::shared_ptr<Renderer> IGraphicsWorld::GetRenderer(
	const std::string& name)
{
	return rendererMap[name];
}

void IGraphicsWorld::AllocateBuffers()
{
	for (const auto& [key, renderer] : rendererMap) {
		renderer->AllocateBuffers();
	}
}

void IGraphicsWorld::Render()
{
	for (const auto& [key, renderer] : rendererMap) {
		renderer->RenderScene(camera);
	}
}

void IGraphicsWorld::SetRendererProjectionAndView(
	const glm::mat4& projection, const glm::mat4& view)
{
	for (const auto& [key, renderer] : rendererMap) {
		renderer->SetProjection(projection);
		renderer->SetView(view);
	}
}

void IGraphicsWorld::AddScene(
	const std::string& name, std::shared_ptr<Scene> scene)
{
	if (sceneMap.contains(name)) return;
	sceneMap[name] = scene;
}
