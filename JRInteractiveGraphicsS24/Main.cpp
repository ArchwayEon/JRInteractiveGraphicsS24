#include <Windows.h>
#include <memory>
#include "GraphicsEnvironment.h"
#include "GraphicsWorld.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	auto glfw = std::make_shared<GraphicsEnvironment>();
	glfw->Init(4, 3);

	bool created = glfw->SetWindow(
		1200, 800, "ETSU Computing Interactive Graphics");
	if (created == false) {
		glfw->ShowNotice("Could not create the window!");
		return -1;
	}

	bool loaded = glfw->InitGlad();
	if (loaded == false) {
		glfw->ShowNotice("Could not initialize GLAD!");
		return -1;
	}

	glfw->SetupGraphics();

	auto world = std::make_shared<GraphicsWorld>(glfw);
	glfw->AddGraphicsWorld("World1", world);
	glfw->SetCurrentWorld("World1");
	glfw->CreateWorlds();

	glfw->Run3D();
	return 0;
}

