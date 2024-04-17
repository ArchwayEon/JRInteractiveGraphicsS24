#include "ObjectMapWorld.h"
#include "GraphicsEnvironment.h"
#include "Generate.h"
#include "HighlightBehavior.h"
#include <glm/gtc/matrix_transform.hpp>
#include "UsefulMacros.h"
#include "PCCuboidGenerator.h"
#include "ChangeColorBehavior.h"

ObjectMapWorld::ObjectMapWorld(std::shared_ptr<GraphicsEnvironment> env) : 
	IGraphicsWorld(env), globalLight{}, localLight{}
{
	_env = env;
	map = std::make_shared<ObjectSpatialMap>(5, 5, 10, 10);
}

void ObjectMapWorld::Create()
{
	CreateScene1();
	CreateScene2();
	CreateScene3();
	CreateRenderers();
}

void ObjectMapWorld::Preupdate()
{
	mainScene = GetScene("Lighting");
	globalLight = mainScene->GetGlobalLight();
	localLight = mainScene->GetLocalLight();
	camera->SetPosition({ 0.0f, 20.0f, 50.0f });
	lookWithMouse = false;
	objectManager->SetBehaviorDefaults();
}

void ObjectMapWorld::Update(float elapsedSeconds)
{
	
	auto& mouse = _env->GetMouseParams();

	_env->PollInputs(elapsedSeconds);
	_env->UpdateWindowSize();
	_env->UpdateMousePosition();

	if (lookWithMouse) {
		camera->SetLookFrame(mouse.spherical.ToMat4());
	}

	//view = camera->GetLookForwardViewMatrix();
	view = camera->GetLookAtViewMatrix({ 0.0f, 5.0f, 0.0f });

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
	std::stringstream ss;
	for (int i = 1; i <= numberOfCrates; i++) {
		ss.str("");
		ss << "Crate" << i;
		auto crate = objectManager->GetGraphicsObject(ss.str());
		crate->SetBehaviorParameters("highlight", hp);
	}

	auto mouseRayLine = GetGraphicsObject("PCMouseRay");
	// Get the mouse ray parameters
	mouseRayStart = mouseRay.GetStartPoint();
	mouseRayEnd = mouseRay.GetPoint(50.0f);
	// Draw a line to show the mouse ray
	auto& mrvb = mouseRayLine->GetVertexBuffer();
	mrvb->Clear();
	mrvb->AddVertexData(6,
		mouseRayStart.x, mouseRayStart.y, mouseRayStart.z,
		1.0f, 1.0f, 1.0f);
	mrvb->AddVertexData(6,
		mouseRayEnd.x, mouseRayEnd.y, mouseRayEnd.z,
		0.0f, 0.0f, 1.0f);

	auto floor = GetGraphicsObject("Floor");
	auto& position = floor->GetPosition();
	floorPlane.Set({ 0.0f, 1.0f, 0.0f }, position.y);

	float offset = floorPlane.GetIntersectionOffset(mouseRay);
	if (offset >= 0) {
		point = mouseRay.GetPoint(offset);
		localLight.position = point;
		localLight.position.y = 6.0f;
		if (localLight.position.x < -25.0f) localLight.position.x = -25.0f;
		if (localLight.position.x > 25.0f) localLight.position.x = 25.0f;
		if (localLight.position.z < -25.0f) localLight.position.z = -25.0f;
		if (localLight.position.z > 25.0f) localLight.position.z = 25.0f;
		row = map->GetRow(point.z);
		col = map->GetCol(point.x);
	}
	else {
		row = -1;
		col = -1;
	}

	mainScene->GetLocalLight().position = localLight.position;
	mainScene->GetLocalLight().intensity = localLight.intensity;
	mainScene->GetGlobalLight().intensity = globalLight.intensity;
	auto lightBulb = GetGraphicsObject("LightBulb");
	lightBulb->SetPosition(localLight.position);
	lightBulb->PointAt(camera->GetPosition());

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
	ImGui::DragFloat3("Intersection point",	(float*)&point, 0.1f);
	ImGui::Text("[Row, Col]: [%d, %d]", row, col);
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
		camera->SetPosition({ 0.0f, 20.0f, 50.0f });
		camera->SetLookFrame(lookFrame);
		lookWithMouse = false;
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		glm::mat4 lookFrame(1.0f);
		lookFrame = glm::rotate(lookFrame, glm::radians(90.0f), { 0, 1, 0 });
		camera->SetPosition({ 50.0f, 20.0f, 0.0f });
		camera->SetLookFrame(lookFrame);
		lookWithMouse = false;
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		glm::mat4 lookFrame(1.0f);
		lookFrame = glm::rotate(lookFrame, glm::radians(180.0f), { 0, 1, 0 });
		camera->SetPosition({ 0.0f, 20.0f, -50.0f });
		camera->SetLookFrame(lookFrame);
		lookWithMouse = false;
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		glm::mat4 lookFrame(1.0f);
		lookFrame = glm::rotate(lookFrame, glm::radians(-90.0f), { 0, 1, 0 });
		camera->SetPosition({ -50.0f, 20.0f, 0.0f });
		camera->SetLookFrame(lookFrame);
		lookWithMouse = false;
		return;
	}
}

void ObjectMapWorld::CreateRenderers()
{
	CreateRenderer1();
	CreateRenderer2();
	CreateRenderer3();
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

	std::stringstream ss;

	float width, height, depth, halfHeight;
	float x, z;
	for (int i = 0; i < numberOfCrates; i++) {
		ss.str("");
		std::shared_ptr<GraphicsObject> crate = std::make_shared<GraphicsObject>();
		width = (float)RANGED_RANDOM_INT(1, 4);
		height = (float)RANGED_RANDOM_INT(1, 4);
		depth = (float)RANGED_RANDOM_INT(1, 4);
		
		auto buffer = Generate::CuboidWithNormals(width, height, depth);
		buffer->AddVertexAttribute("position", 0, 3, 0);
		buffer->AddVertexAttribute("vertexColor", 1, 4, 3);
		buffer->AddVertexAttribute("vertexNormal", 2, 3, 7);
		buffer->AddVertexAttribute("texCoord", 3, 2, 10);
		buffer->SetTexture(crateTexture);
		crate->SetVertexBuffer(buffer);
		crate->CreateBoundingBox(width, height, depth);
		auto hb2 = std::make_shared<HighlightBehavior>(crate);
		crate->AddBehavior("highlight", hb2);
		halfHeight = height / 2;
		x = (float)RANGED_RANDOM(-22.0f, 22.0f);
		z = (float)RANGED_RANDOM(-22.0f, 22.0f);
		crate->SetPosition(glm::vec3(x, halfHeight, z));
		scene->AddObject(crate);
		ss << "Crate" << i+1;
		AddObject(ss.str(), crate);
	}

	std::shared_ptr<Texture> floorTexture = std::make_shared<Texture>();
	floorTexture->LoadTextureDataFromFile("floor.jpg");
	_env->AddTexture("Floor", floorTexture);

	std::shared_ptr<GraphicsObject> floor = std::make_shared<GraphicsObject>();
	auto buffer = Generate::XZPlaneWithNormals(
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

void ObjectMapWorld::CreateScene2()
{
	auto shader = _env->CreateShader(
		"Basic", "basic.vert.glsl", "basic.frag.glsl");
	shader->AddUniform("projection");
	shader->AddUniform("world");
	shader->AddUniform("view");

	auto scene = std::make_shared<Scene>();
	AddScene("Basic", scene);

	std::shared_ptr<GraphicsObject> pcMouseRay =
		std::make_shared<GraphicsObject>();
	pcMouseRay->CreateVertexBuffer(6);
	auto& mrvb = pcMouseRay->GetVertexBuffer();
	mrvb->SetPrimitiveType(GL_LINES);
	mrvb->AddVertexData(6, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	mrvb->AddVertexData(6, 0.0f, 0.0f, -50.0f, 1.0f, 1.0f, 1.0f);
	mrvb->AddVertexAttribute("position", 0, 3, 0);
	mrvb->AddVertexAttribute("color", 1, 3, 3);
	pcMouseRay->CreateIndexBuffer();
	pcMouseRay->GetIndexBuffer()->AddIndexData(2, 0, 1);
	pcMouseRay->SetUpDynamicBuffers(4, 4);
	pcMouseRay->SetPosition({ 0.0f, 0.0f, 0.0f });
	scene->AddObject(pcMouseRay);
	AddObject("PCMouseRay", pcMouseRay);

	float x = -20.0f, y = 5.0f, z = -20.0f;
	std::stringstream ss;
	for (int i = 0; i < 25; i++) {
		ss.str("");
		std::shared_ptr<GraphicsObject> pcLineCuboid =
			std::make_shared<GraphicsObject>();
		pcLineCuboid->SetGenerator(
			std::make_shared<PCCuboidGenerator>(pcLineCuboid));
		auto cparams = std::make_shared<PCCuboidParams>();
		cparams->width = 10.0f;
		cparams->height = 10.0f;
		cparams->depth = 10.0f;
		pcLineCuboid->GetMaterial().color = { 1.0f, 0.0f, 1.0f };
		pcLineCuboid->GetGenerator()->SetParameters(cparams);
		pcLineCuboid->Generate(UseDynamicBuffers);
		pcLineCuboid->SetPosition({ x, y, z });
		x += 10;
		if (x >= 25) {
			x = -20.0f;
			z += 10;
		}
		pcLineCuboid->CreateBoundingBox(10.0f, 10.0f, 10.0f);
		auto ccb = std::make_shared<ChangeColorBehavior>(
			glm::vec3(1.0f, 0.0f, 0.0f));
		ccb->SetObject(pcLineCuboid);
		pcLineCuboid->AddBehavior("ChangeColor", ccb);
		scene->AddObject(pcLineCuboid);
		ss << "PCLineCuboid" << i + 1;
		AddObject(ss.str(), pcLineCuboid);
	}

}

void ObjectMapWorld::CreateScene3()
{
	auto shader = _env->CreateShader(
		"BasicTexture", "texture.vert.glsl", "texture.frag.glsl");

	shader->AddUniform("projection");
	shader->AddUniform("world");
	shader->AddUniform("view");
	shader->AddUniform("texUnit");
	auto scene = std::make_shared<Scene>();
	AddScene("BasicTexture", scene);

	std::shared_ptr<Texture> lightBulbTexture = std::make_shared<Texture>();
	lightBulbTexture->LoadTextureDataFromFile("lightbulb.png");
	_env->AddTexture("LightBulb", lightBulbTexture);

	std::shared_ptr<GraphicsObject> lightBulb =
		std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> buffer =
		Generate::XYPlane(2.0f, 2.0f);
	buffer->AddVertexAttribute("position", 0, 3, 0);
	buffer->AddVertexAttribute("vertexColor", 1, 3, 3);
	buffer->AddVertexAttribute("texCoord", 2, 2, 6);
	buffer->SetTexture(lightBulbTexture);
	lightBulb->SetVertexBuffer(buffer);
	scene->AddObject(lightBulb);
	AddObject("LightBulb", lightBulb);
}

void ObjectMapWorld::CreateRenderer1()
{
	CreateRenderer("Lighting", _env->GetShader("Lighting"));
	GetRenderer("Lighting")->SetScene(GetScene("Lighting"));
}

void ObjectMapWorld::CreateRenderer2()
{
	CreateRenderer("Basic", _env->GetShader("Basic"));
	GetRenderer("Basic")->SetScene(GetScene("Basic"));
}

void ObjectMapWorld::CreateRenderer3()
{
	CreateRenderer("BasicTexture", _env->GetShader("BasicTexture"));
	GetRenderer("BasicTexture")->SetScene(GetScene("BasicTexture"));
}
