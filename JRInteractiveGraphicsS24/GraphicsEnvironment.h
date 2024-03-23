#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <sstream>
#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Renderer.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "GraphicsStructures.h"
#include "Ray.h"

class GraphicsEnvironment : public BaseObject
{
private:
	GLFWwindow* window;
	std::shared_ptr<ObjectManager> objectManager;
	std::unordered_map<std::string, std::shared_ptr<Renderer>> rendererMap;
	Camera camera;
	MouseParams mouse;
	static GraphicsEnvironment* self;
	bool lookWithMouse = true;
	bool correctGamma = false;

public:
	GraphicsEnvironment();
	~GraphicsEnvironment();

	GLFWwindow* GetWindow() const { return window; }
	void Init(unsigned int majorVersion, unsigned int minorVersion);
	bool SetWindow(unsigned int width, unsigned int height, const std::string& title);
	bool InitGlad();
	void SetupGraphics();
	inline ImGuiIO& GetImGuiIO() {
		return ImGui::GetIO();
	}
	void CreateRenderer(const std::string& name, std::shared_ptr<Shader> shader);
	std::shared_ptr<Renderer> GetRenderer(const std::string& name);
	void StaticAllocate();
	void Render();
	void Run2D();
	void Run3D();
	void SetRendererProjectionAndView(const glm::mat4& projection, const glm::mat4& view);
	void AddObject(const std::string& name, std::shared_ptr<GraphicsObject> object);

	MouseParams& GetMouseParams() { return mouse; }

	Ray GetMouseRay(const glm::mat4& projection, const glm::mat4& view);
public:
	static void OnWindowSizeChanged(GLFWwindow* window, int width, int height);
	static void OnMouseMove(GLFWwindow* window, double mouseX, double mouseY);

private:
	void ShutDown();
	void ProcessInput(double elapsedSeconds);
	static glm::mat4 CreateViewMatrix(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up);
};

