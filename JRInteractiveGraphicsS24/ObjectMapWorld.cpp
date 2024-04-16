#include "ObjectMapWorld.h"
#include "GraphicsEnvironment.h"
#include "Generate.h"
#include "HighlightBehavior.h"
#include <glm/gtc/matrix_transform.hpp>

ObjectMapWorld::ObjectMapWorld(std::shared_ptr<GraphicsEnvironment> env) : 
	IGraphicsWorld(env), globalLight{}, localLight{}
{
	_env = env;
}

void ObjectMapWorld::Create()
{
	CreateScene1();
	CreateRenderers();
}

void ObjectMapWorld::Preupdate()
{
	mainScene = GetScene("Lighting");
	globalLight = mainScene->GetGlobalLight();
	localLight = mainScene->GetLocalLight();
	camera->SetPosition({ 0.0f, 2.0f, 30.0f });
	objectManager->SetBehaviorDefaults();
}

void ObjectMapWorld::Update(float elapsedSeconds)
{
	auto crate = objectManager->GetGraphicsObject("Crate");
	auto& mouse = _env->GetMouseParams();

	_env->PollInputs(elapsedSeconds);
	_env->UpdateWindowSize();
	_env->UpdateMousePosition();

	if (lookWithMouse) {
		camera->SetLookFrame(mouse.spherical.ToMat4());
	}

	view = camera->GetLookForwardViewMatrix();

	if (mouse.windowWidth >= mouse.windowHeight) {
		aspectRatio = mouse.windowWidth / (mouse.windowHeight * 1.0f);
	}
	else {
		aspectRatio = mouse.windowHeight / (mouse.windowWidth * 1.0f);
	}
	projection = glm::perspective(
		glm::radians(fieldOfView), aspectRatio, nearPlane, farPlane);

	SetRendererProjectionAndView(projection, view);

	mouseRay = _env->GetMouseRay(projection, view);

	std::shared_ptr<HighlightParams> hp = std::make_shared<HighlightParams>();
	hp->ray = &mouseRay;
	crate->SetBehaviorParameters("highlight", hp);

	objectManager->Update(elapsedSeconds);
}

void ObjectMapWorld::UI(ImGuiIO& io)
{
	auto& mouse = _env->GetMouseParams();
	ImGui::Text(Logger::GetLog().c_str());
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
		1000.0f / io.Framerate, io.Framerate);
	ImGui::Text("Mouse SC: (%.3f, %.3f)", mouse.x, mouse.y);
	ImGui::DragFloat3("Local light position",
		(float*)&localLight.position, 0.1f);
	ImGui::Checkbox("Look with mouse", &lookWithMouse);
	ImGui::SliderFloat("Global Intensity", &globalLight.intensity, 0, 1);
	ImGui::SliderFloat("Local Intensity", &localLight.intensity, 0, 1);
}

void ObjectMapWorld::OnMouseButton(int button, int action, int mods)
{
}

void ObjectMapWorld::OnKey(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS) {
		lookWithMouse = !lookWithMouse;
		return;
	}
}

void ObjectMapWorld::PollInputs(float elapsedSeconds)
{
	auto window = _env->GetWindow();
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->MoveForward(elapsedSeconds);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->MoveBackward(elapsedSeconds);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->MoveLeft(elapsedSeconds);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->MoveRight(elapsedSeconds);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		camera->MoveUp(elapsedSeconds);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		camera->MoveDown(elapsedSeconds);
		return;
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		glm::mat4 lookFrame(1.0f);
		camera->SetPosition({ 0.0f, 5.0f, 30.0f });
		camera->SetLookFrame(lookFrame);
		lookWithMouse = false;
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		glm::mat4 lookFrame(1.0f);
		lookFrame = glm::rotate(lookFrame, glm::radians(90.0f), { 0, 1, 0 });
		camera->SetPosition({ 30.0f, 5.0f, 0.0f });
		camera->SetLookFrame(lookFrame);
		lookWithMouse = false;
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		glm::mat4 lookFrame(1.0f);
		lookFrame = glm::rotate(lookFrame, glm::radians(180.0f), { 0, 1, 0 });
		camera->SetPosition({ 0.0f, 5.0f, -30.0f });
		camera->SetLookFrame(lookFrame);
		lookWithMouse = false;
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		glm::mat4 lookFrame(1.0f);
		lookFrame = glm::rotate(lookFrame, glm::radians(-90.0f), { 0, 1, 0 });
		camera->SetPosition({ -30.0f, 5.0f, 0.0f });
		camera->SetLookFrame(lookFrame);
		lookWithMouse = false;
		return;
	}
}

void ObjectMapWorld::CreateRenderers()
{
	CreateRenderer1();
}

void ObjectMapWorld::CreateScene1()
{
	auto shader = _env->CreateShader(
		"Lighting", "lighting.vert.glsl", "lighting.frag.glsl");
	shader->AddUniform("projection");
	shader->AddUniform("world");
	shader->AddUniform("view");
	shader->AddUniform("texUnit");
	shader->AddUniform("materialAmbientIntensity");
	shader->AddUniform("materialSpecularIntensity");
	shader->AddUniform("materialShininess");
	shader->AddUniform("globalLightPosition");
	shader->AddUniform("globalLightColor");
	shader->AddUniform("globalLightIntensity");
	shader->AddUniform("localLightPosition");
	shader->AddUniform("localLightColor");
	shader->AddUniform("localLightIntensity");
	shader->AddUniform("localLightAttenuationCoef");
	shader->AddUniform("viewPosition");

	std::shared_ptr<Scene> scene = std::make_shared<Scene>();
	AddScene("Lighting", scene);

	std::shared_ptr<Texture> crateTexture = std::make_shared<Texture>();
	crateTexture->LoadTextureDataFromFile("crate.jpg");
	_env->AddTexture("Crate", crateTexture);

	std::shared_ptr<GraphicsObject> crate = std::make_shared<GraphicsObject>();
	auto buffer = Generate::CuboidWithNormals(3.0f, 3.0f, 3.0f);
	buffer->AddVertexAttribute("position", 0, 3, 0);
	buffer->AddVertexAttribute("vertexColor", 1, 4, 3);
	buffer->AddVertexAttribute("vertexNormal", 2, 3, 7);
	buffer->AddVertexAttribute("texCoord", 3, 2, 10);
	buffer->SetTexture(crateTexture);
	crate->SetVertexBuffer(buffer);
	crate->CreateBoundingBox(3.0f, 3.0f, 3.0f);
	auto hb2 = std::make_shared<HighlightBehavior>(crate);
	crate->AddBehavior("highlight", hb2);
	crate->SetPosition(glm::vec3(-20.0f, 5.0f, -5.0f));
	scene->AddObject(crate);
	AddObject("Crate", crate);

	std::shared_ptr<Texture> floorTexture = std::make_shared<Texture>();
	floorTexture->LoadTextureDataFromFile("floor.jpg");
	_env->AddTexture("Floor", floorTexture);

	std::shared_ptr<GraphicsObject> floor = std::make_shared<GraphicsObject>();
	buffer = Generate::XZPlaneWithNormals(
		50.0f, 50.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, { 10.0f, 10.0f });
	buffer->AddVertexAttribute("position", 0, 3, 0);
	buffer->AddVertexAttribute("vertexColor", 1, 4, 3);
	buffer->AddVertexAttribute("vertexNormal", 2, 3, 7);
	buffer->AddVertexAttribute("texCoord", 3, 2, 10);
	buffer->SetTexture(floorTexture);
	floor->SetVertexBuffer(buffer);
	floor->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	scene->AddObject(floor);
	AddObject("Floor", floor);

	scene->GetLocalLight().position = { 0.0f, 5.0f, 5.0f };
	scene->GetLocalLight().intensity = 0.5f;
}

void ObjectMapWorld::CreateRenderer1()
{
	CreateRenderer("Lighting", _env->GetShader("Lighting"));
	GetRenderer("Lighting")->SetScene(GetScene("Lighting"));
}
