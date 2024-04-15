#pragma once
#include <memory>
#include "Camera.h"
#include <glm/glm.hpp>
#include "Ray.h"
#include "Scene.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "IGraphicsWorld.h"

class GraphicsEnvironment;

class GraphicsWorld : public IGraphicsWorld
{
private:
	bool correctGamma = false;
	bool lookWithMouse = false;
	float aspectRatio = 0.0f;
	float nearPlane = 1.0f;
	float farPlane = 100.0f;
	float fieldOfView = 60;
	glm::mat4 cubeReferenceFrame;
	float cubeYAngle = 0;
	float cubeXAngle = 0;
	float cubeZAngle = 0;
	Light localLight{}, globalLight{};
	GeometricPlane floorPlane;
	float offset = 0.0f;
	glm::vec3 spherePos = { 0.0f, 0.0f, 0.0f };
	bool isSphereOverlapping = false;
	float cuboid1Deg = 0.0f;
	float cuboid2Deg = 0.0f;
	glm::vec3 cuboidPos = { 0.0f, 0.0f, 0.0f };
	bool isCuboidOverlapping = false;
	glm::vec3 mouseRayStart = { 0.0f, 0.0f, 0.0f };
	glm::vec3 mouseRayEnd = { 0.0f, 0.0f, 0.0f };

public:
	GraphicsWorld(std::shared_ptr<GraphicsEnvironment> env);
	void Create() override;
	void Preupdate() override;;
	void Update(float elapsedSeconds) override;;
	void UI(ImGuiIO& io) override;
	void OnMouseButton(int button, int action, int mods) override;
	void OnKey(int key, int scancode, int action, int mods) override;
	void PollInputs(float elapsedSeconds) override;
private:
	void CreateRenderers() override;;
	// Specializations for each world.
	void CreateScene1();
	void CreateScene2();
	void CreateScene3();

	void CreateRenderer1();
	void CreateRenderer2();
	void CreateRenderer3();
};

