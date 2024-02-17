#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <sstream>
#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class GraphicsEnvironment
{
private:
	GLFWwindow* window;
	std::stringstream logger;

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
public:
	static void OnWindowSizeChanged(GLFWwindow* window, int width, int height);
};

