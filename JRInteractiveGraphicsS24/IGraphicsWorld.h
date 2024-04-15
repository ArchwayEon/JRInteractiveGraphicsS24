#pragma once
#include <memory>
#include "Camera.h"
#include <glm/glm.hpp>
#include "Ray.h"
#include "Scene.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class GraphicsEnvironment;

class IGraphicsWorld
{
protected:
	std::shared_ptr<GraphicsEnvironment> _env;
	std::shared_ptr<Camera> camera = nullptr;
	std::shared_ptr<Scene> mainScene = nullptr;
	glm::mat4 view;
	glm::mat4 projection;
	Ray mouseRay;

public:
	IGraphicsWorld(std::shared_ptr<GraphicsEnvironment> env);
	virtual ~IGraphicsWorld() = default;
	void SetCamera(std::shared_ptr<Camera> camera) {
		this->camera = camera;
	}
	virtual void Create() = 0;
	virtual void Preupdate() = 0;
	virtual void Update(float elapsedSeconds) = 0;
	virtual void UI(ImGuiIO& io) = 0;
	virtual void OnMouseButton(int button, int action, int mods) = 0;
	virtual void OnKey(int key, int scancode, int action, int mods) = 0;
	virtual void PollInputs(float elapsedSeconds) = 0;
private:
	virtual void CreateRenderers() = 0;
};

