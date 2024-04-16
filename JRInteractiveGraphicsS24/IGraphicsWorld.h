#pragma once
#include <memory>
#include "Camera.h"
#include <glm/glm.hpp>
#include "Ray.h"
#include "Scene.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "ObjectManager.h"

class GraphicsEnvironment;
class Shader;
class Renderer;

class IGraphicsWorld
{
protected:
	std::unordered_map<std::string, std::shared_ptr<Renderer>> rendererMap;
	std::unordered_map<std::string, std::shared_ptr<Scene>> sceneMap;
	std::shared_ptr<ObjectManager> objectManager;
	std::shared_ptr<GraphicsEnvironment> _env;
	std::shared_ptr<Camera> camera = nullptr;
	std::shared_ptr<Scene> mainScene = nullptr;
	glm::mat4 view;
	glm::mat4 projection;
	Ray mouseRay;
	bool lookWithMouse = false;
	float aspectRatio = 0.0f;
	float nearPlane = 1.0f;
	float farPlane = 100.0f;
	float fieldOfView = 60;

public:
	IGraphicsWorld(std::shared_ptr<GraphicsEnvironment> env);
	virtual ~IGraphicsWorld() = default;
	virtual void SetCamera(std::shared_ptr<Camera> camera) {
		this->camera = camera;
	}
	virtual void AddObject(
		const std::string& name, std::shared_ptr<GraphicsObject> object);
	virtual std::shared_ptr<GraphicsObject> GetGraphicsObject(
		const std::string& name) {
		return objectManager->GetGraphicsObject(name);
	}
	virtual void CreateRenderer(
		const std::string& name, std::shared_ptr<Shader> shader);
	virtual std::shared_ptr<Renderer> GetRenderer(const std::string& name);
	virtual void SetRendererProjectionAndView(
		const glm::mat4& projection, const glm::mat4& view);
	virtual void AddScene(
		const std::string& name, std::shared_ptr<Scene> scene);
	virtual std::shared_ptr<Scene> GetScene(const std::string& name) {
		if (sceneMap.contains(name) == false) return nullptr;
		return sceneMap[name];
	}
	virtual void AllocateBuffers();
	virtual void Create() = 0;
	virtual void Preupdate() = 0;
	virtual void Update(float elapsedSeconds) = 0;
	virtual void UI(ImGuiIO& io) = 0;
	virtual void OnMouseButton(int button, int action, int mods) = 0;
	virtual void OnKey(int key, int scancode, int action, int mods) = 0;
	virtual void PollInputs(float elapsedSeconds) = 0;
	virtual void Render();
private:
	virtual void CreateRenderers() = 0;
};

