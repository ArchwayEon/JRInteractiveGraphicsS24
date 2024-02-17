#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <sstream>
#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Renderer.h"

class GraphicsEnvironment
{
private:
	GLFWwindow* window;
	std::stringstream logger;
	std::unordered_map<std::string, std::shared_ptr<Renderer>> rendererMap;

public:
	GraphicsEnvironment();
	~GraphicsEnvironment();

	GLFWwindow* GetWindow() const { return window; }
	void Init(unsigned int majorVersion, unsigned int minorVersion);
	inline const std::string GetLog() const {
		return logger.str();
	}
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
	void Run();
	void SetRendererProjectionAndView(const glm::mat4& projection, const glm::mat4& view);
public:
	static void OnWindowSizeChanged(GLFWwindow* window, int width, int height);

private:
	void ProcessInput();
	static glm::mat4 CreateViewMatrix(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up);
};

