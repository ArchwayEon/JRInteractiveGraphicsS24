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
#include "GraphicsWorld.h"

class GraphicsEnvironment
{
private:
	GLFWwindow* window;
	std::shared_ptr<ObjectManager> objectManager;
	std::unordered_map<std::string, std::shared_ptr<Renderer>> rendererMap;
	std::unordered_map<std::string, std::shared_ptr<Shader>> shaderMap;
	std::unordered_map<std::string, std::shared_ptr<Scene>> sceneMap;
	std::unordered_map<std::string, std::shared_ptr<Texture>> textureMap;
	std::unordered_map<std::string, std::shared_ptr<GraphicsWorld>> worldMap;
	Camera camera;
	MouseParams mouse;
	static GraphicsEnvironment* self;
	bool lookWithMouse = true;
	bool correctGamma = false;
	glm::mat4 view;
	glm::mat4 projection;
	Ray mouseRay;
	std::string testStr;

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
	void Allocate();
	void Render();
	void Run2D();
	void Run3D();
	void SetRendererProjectionAndView(const glm::mat4& projection, const glm::mat4& view);
	void AddObject(const std::string& name, std::shared_ptr<GraphicsObject> object);
	std::shared_ptr<GraphicsObject> GetObject(const std::string& name) {
		return objectManager->GetObject(name);
	}

	MouseParams& GetMouseParams() { return mouse; }

	Ray GetMouseRay(const glm::mat4& projection, const glm::mat4& view);
	const Ray& GetMouseRay() const { return mouseRay; }

	void UpdateMousePosition();
	void AddShader(const std::string& name, std::shared_ptr<Shader> shader);
	std::shared_ptr<Shader> GetShader(const std::string& name) {
		if (shaderMap.contains(name) == false) return nullptr;
		return shaderMap[name];
	}
	void AddScene(const std::string& name, std::shared_ptr<Scene> scene);
	std::shared_ptr<Scene> GetScene(const std::string& name) {
		if (sceneMap.contains(name) == false) return nullptr;
		return sceneMap[name];
	}
	void AddTexture(const std::string& name, std::shared_ptr<Texture> texture);
	void AddGraphicsWorld(
		const std::string& name, std::shared_ptr<GraphicsWorld> world);

public:
	static void OnWindowSizeChanged(GLFWwindow* window, int width, int height);
	static void OnMouseMove(GLFWwindow* window, double mouseX, double mouseY);
	static void OnMouseButton(GLFWwindow* window, int button, int action, int mods);
	static void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	void ShutDown();
	void ProcessInput(double elapsedSeconds);
	static glm::mat4 CreateViewMatrix(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up);
};

