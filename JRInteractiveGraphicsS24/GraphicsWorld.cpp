#include "GraphicsWorld.h"
#include "GraphicsEnvironment.h"
#include "TextFile.h"
#include "Generate.h"
#include "HighlightBehavior.h"
#include "RotateBehavior.h"
#include "PCSphereGenerator.h"
#include "ChangeColorBehavior.h"
#include "PCCuboidGenerator.h"

#include <glm/gtc/matrix_transform.hpp>
#include "GraphicsStructures.h"
#include "RotateAnimation.h"
#include "MoveAnimation.h"
#include "RotateXYZBehavior.h"

GraphicsWorld::GraphicsWorld(std::shared_ptr<GraphicsEnvironment> env) :
	IGraphicsWorld(env), cubeReferenceFrame(1.0f), globalLight{}, localLight{}
{
	_env = env;
}

void GraphicsWorld::Create()
{
	CreateScene1();
	CreateScene2();
	CreateScene3();
	CreateRenderers();
}

void GraphicsWorld::Preupdate()
{
	mainScene = _env->GetScene("Lighting");
	globalLight = mainScene->GetGlobalLight();
	localLight = mainScene->GetLocalLight();
	spherePos =
		GetGraphicsObject("PCLinesSphere1")->GetPosition();
	cuboidPos =
		GetGraphicsObject("PCLineCuboid1")->GetPosition();
	_env->GetObjectManager()->SetBehaviorDefaults();

	auto crate = GetGraphicsObject("Crate");
	std::shared_ptr<RotateAnimation> rotateAnimation =
		std::make_shared<RotateAnimation>();
	rotateAnimation->SetObject(crate);
	crate->SetAnimation(rotateAnimation);

	auto cubeWorld = GetGraphicsObject("World");
	std::shared_ptr<MoveAnimation> moveAnimation =
		std::make_shared<MoveAnimation>();
	moveAnimation->SetObject(cubeWorld);
	cubeWorld->SetAnimation(moveAnimation);

	camera->SetPosition({ 0.0f, 2.0f, 30.0f });
}

void GraphicsWorld::Update(float elapsedSeconds)
{
	auto cube = GetGraphicsObject("TexturedCube");
	auto lightBulb = GetGraphicsObject("LightBulb");
	auto floor = GetGraphicsObject("Floor");
	auto cylinder = GetGraphicsObject("PCLinesCylinder");
	auto sphere1 = GetGraphicsObject("PCLinesSphere1");
	auto sphere2 = GetGraphicsObject("PCLinesSphere2");
	auto cuboid1 = GetGraphicsObject("PCLineCuboid1");
	auto cuboid2 = GetGraphicsObject("PCLineCuboid2");
	auto crate = GetGraphicsObject("Crate");
	auto cubeWorld = GetGraphicsObject("World");
	auto mouseRayLine = GetGraphicsObject("PCMouseRay");

	auto& mouse = _env->GetMouseParams();

	_env->PollInputs(elapsedSeconds);
	_env->UpdateWindowSize();
	_env->UpdateMousePosition();

	if (correctGamma) {
		glEnable(GL_FRAMEBUFFER_SRGB);
	}
	else {
		glDisable(GL_FRAMEBUFFER_SRGB);
	}

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

	_env->SetRendererProjectionAndView(projection, view);

	mouseRay = _env->GetMouseRay(projection, view);

	lightBulb->SetPosition(localLight.position);
	lightBulb->PointAt(camera->GetPosition());

	auto& position = floor->GetPosition();
	floorPlane.Set({ 0.0f, 1.0f, 0.0f }, position.y);

	offset = floorPlane.GetIntersectionOffset(mouseRay);
	if (offset >= 0) {
		glm::vec3 point = mouseRay.GetPoint(offset);
		float y = cylinder->GetPosition().y;
		cylinder->SetPosition({ point.x, y, point.z });
	}

	sphere1->SetPosition(spherePos);
	isSphereOverlapping = sphere1->OverlapsWithBoundingSphere(*sphere2);
	sphere1->SetIsOverlapping(isSphereOverlapping);
	sphere2->SetIsOverlapping(isSphereOverlapping);

	std::shared_ptr<RotateParams> rp1 = std::make_shared<RotateParams>();
	rp1->axis = { 0.0f, 1.0f, 0.0f };
	rp1->degrees = cuboid1Deg;
	std::shared_ptr<RotateParams> rp2 = std::make_shared<RotateParams>();
	rp2->axis = { 0.0f, 1.0f, 0.0f };
	rp2->degrees = cuboid2Deg;
	cuboid1->SetBehaviorParameters("rotateY", rp1);
	cuboid2->SetBehaviorParameters("rotateY", rp2);
	cuboid1->SetPosition(cuboidPos);
	isCuboidOverlapping = cuboid1->OverlapsWithBoundingBox(*cuboid2);
	cuboid1->SetIsOverlapping(isCuboidOverlapping);
	cuboid2->SetIsOverlapping(isCuboidOverlapping);

	std::shared_ptr<RotateXYZParams> cubeXYZ = std::make_shared<RotateXYZParams>();
	cubeXYZ->degreesX = cubeXAngle;
	cubeXYZ->degreesY = cubeYAngle;
	cubeXYZ->degreesZ = cubeZAngle;
	cube->SetBehaviorParameters("rotateXYZ", cubeXYZ);

	std::shared_ptr<HighlightParams> hp =
		std::make_shared<HighlightParams>();
	hp->ray = &mouseRay;
	cube->SetBehaviorParameters("highlight", hp);
	crate->SetBehaviorParameters("highlight", hp);
	cubeWorld->SetBehaviorParameters("highlight", hp);

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

	objectManager->Update(elapsedSeconds);
}

void GraphicsWorld::UI(ImGuiIO& io)
{
	auto& mouse = _env->GetMouseParams();
	ImGui::Text(Logger::GetLog().c_str());
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
		1000.0f / io.Framerate, io.Framerate);
	ImGui::Text("Offset: %.3f", offset);
	ImGui::Text("Mouse NSC: (%.3f, %.3f)", mouse.nsx, mouse.nsy);
	ImGui::Text("Mouse SC: (%.3f, %.3f)", mouse.x, mouse.y);
	ImGui::Text("Mouse Ray Start: (%.3f, %.3f, %.3f)",
		mouseRayStart.x, mouseRayStart.y, mouseRayStart.z);
	ImGui::Text("Mouse Ray End: (%.3f, %.3f, %.3f)",
		mouseRayEnd.x, mouseRayEnd.y, mouseRayEnd.z);
	ImGui::DragFloat3("Local light position",
		(float*)&localLight.position, 0.1f);
	ImGui::Checkbox("Correct gamma", &correctGamma);
	ImGui::Checkbox("Look with mouse", &lookWithMouse);
	ImGui::SliderFloat("X Angle", &cubeXAngle, 0, 360);
	ImGui::SliderFloat("Y Angle", &cubeYAngle, 0, 360);
	ImGui::SliderFloat("Z Angle", &cubeZAngle, 0, 360);
	ImGui::SliderFloat("Global Intensity", &globalLight.intensity, 0, 1);
	ImGui::SliderFloat("Local Intensity", &localLight.intensity, 0, 1);
	ImGui::SliderFloat("Sphere X Position", &spherePos.x, -5.0f, 5.0f);
	ImGui::Text("Is sphere overlapping: %s", isSphereOverlapping ? "YES" : "NO");
	ImGui::SliderFloat("Cuboid 1 Y Degrees", &cuboid1Deg, 0, 360);
	ImGui::SliderFloat("Cuboid 2 Y Degrees", &cuboid2Deg, 0, 360);
	ImGui::SliderFloat("Cuboid X Position", &cuboidPos.x, -5.0f, 5.0f);
	ImGui::Text("Is cuboid overlapping: %s", isCuboidOverlapping ? "YES" : "NO");
}

void GraphicsWorld::CreateScene1()
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
	_env->AddScene("Lighting", scene);

	std::shared_ptr<Texture> rgbwTexture = std::make_shared<Texture>();
	rgbwTexture->SetDimension(4, 4);
	unsigned char data[] = {
		255, 255, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 255, 255, 255, 255,
		0, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 0, 255,
		0, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 0, 255,
		255, 255, 255, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 255, 255, 255
	};
	rgbwTexture->SetTextureData(64, data);
	_env->AddTexture("RGBW", rgbwTexture);

	std::shared_ptr<GraphicsObject> texturedCube =
		std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> buffer =
		Generate::CuboidWithNormals(10.0f, 5.0f, 5.0f);
	buffer->AddVertexAttribute("position", 0, 3, 0);
	buffer->AddVertexAttribute("vertexColor", 1, 4, 3);
	buffer->AddVertexAttribute("vertexNormal", 2, 3, 7);
	buffer->AddVertexAttribute("texCoord", 3, 2, 10);
	buffer->SetTexture(rgbwTexture);
	texturedCube->SetVertexBuffer(buffer);
	texturedCube->CreateBoundingBox(10.0f, 5.0f, 5.0f);
	auto hb1 = std::make_shared<HighlightBehavior>(texturedCube);
	texturedCube->AddBehavior("highlight", hb1);
	auto rb = std::make_shared<RotateXYZBehavior>(texturedCube);
	rb->SetUpParameters();
	texturedCube->AddBehavior("rotateXYZ", rb);
	scene->AddObject(texturedCube);
	AddObject("TexturedCube", texturedCube);

	std::shared_ptr<Texture> crateTexture = std::make_shared<Texture>();
	crateTexture->LoadTextureDataFromFile("crate.jpg");
	_env->AddTexture("Crate", crateTexture);

	std::shared_ptr<GraphicsObject> crate = std::make_shared<GraphicsObject>();
	buffer = Generate::CuboidWithNormals(10.0f, 10.0f, 10.0f);
	buffer->AddVertexAttribute("position", 0, 3, 0);
	buffer->AddVertexAttribute("vertexColor", 1, 4, 3);
	buffer->AddVertexAttribute("vertexNormal", 2, 3, 7);
	buffer->AddVertexAttribute("texCoord", 3, 2, 10);
	buffer->SetTexture(crateTexture);
	crate->SetVertexBuffer(buffer);
	crate->CreateBoundingBox(10.0f, 10.0f, 10.0f);
	auto hb2 = std::make_shared<HighlightBehavior>(crate);
	crate->AddBehavior("highlight", hb2);
	crate->SetPosition(glm::vec3(-20.0f, 5.0f, -5.0f));
	scene->AddObject(crate);
	AddObject("Crate", crate);

	std::shared_ptr<Texture> worldTexture = std::make_shared<Texture>();
	worldTexture->LoadTextureDataFromFile("world.jpg");
	_env->AddTexture("World", worldTexture);

	std::shared_ptr<GraphicsObject> world = std::make_shared<GraphicsObject>();
	buffer = Generate::CuboidWithNormals(3.0f, 3.0f, 3.0f);
	buffer->AddVertexAttribute("position", 0, 3, 0);
	buffer->AddVertexAttribute("vertexColor", 1, 4, 3);
	buffer->AddVertexAttribute("vertexNormal", 2, 3, 7);
	buffer->AddVertexAttribute("texCoord", 3, 2, 10);
	buffer->SetTexture(worldTexture);
	world->SetVertexBuffer(buffer);
	world->CreateBoundingBox(3.0f, 3.0f, 3.0f);
	auto hb3 = std::make_shared<HighlightBehavior>(world);
	world->AddBehavior("highlight", hb3);
	world->SetPosition(glm::vec3(20.0f, 1.5f, 22.0f));
	scene->AddObject(world);
	AddObject("World", world);

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

void GraphicsWorld::CreateScene2()
{
	auto shader = _env->CreateShader(
		"BasicTexture", "texture.vert.glsl", "texture.frag.glsl");

	shader->AddUniform("projection");
	shader->AddUniform("world");
	shader->AddUniform("view");
	shader->AddUniform("texUnit");
	auto scene = std::make_shared<Scene>();
	_env->AddScene("BasicTexture", scene);

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

void GraphicsWorld::CreateScene3()
{
	auto shader = _env->CreateShader(
		"Basic", "basic.vert.glsl", "basic.frag.glsl");
	shader->AddUniform("projection");
	shader->AddUniform("world");
	shader->AddUniform("view");

	auto scene = std::make_shared<Scene>();
	_env->AddScene("Basic", scene);

	std::shared_ptr<GraphicsObject> pcLinesCircle =
		std::make_shared<GraphicsObject>();
	pcLinesCircle->CreateVertexBuffer(6);
	pcLinesCircle->CreateIndexBuffer();
	auto& vertexBuffer = pcLinesCircle->GetVertexBuffer();
	vertexBuffer->SetPrimitiveType(GL_LINES);
	auto& indexBuffer = pcLinesCircle->GetIndexBuffer();
	Generate::XZLineCircle(vertexBuffer, 2.0f, { 1.0f, 1.0f, 0.0f }, 0.0f, 10);
	vertexBuffer->AddVertexAttribute("position", 0, 3, 0);
	vertexBuffer->AddVertexAttribute("color", 1, 3, 3);
	Generate::LineCircleIndexes(indexBuffer, 36, true);
	pcLinesCircle->SetPosition({ 0.0f, 1.0f, 7.0f });
	scene->AddObject(pcLinesCircle);
	AddObject("PCLinesCircle", pcLinesCircle);

	std::shared_ptr<GraphicsObject> pcLinesCylinder =
		std::make_shared<GraphicsObject>();
	pcLinesCylinder->CreateVertexBuffer(6);
	pcLinesCylinder->CreateIndexBuffer();
	auto& cylinderVertexBuffer = pcLinesCylinder->GetVertexBuffer();
	cylinderVertexBuffer->SetPrimitiveType(GL_LINES);
	Generate::LineCylinder(cylinderVertexBuffer, 4.0f, 2.0f, { 1.0f, 0.0f, 1.0f }, 10);
	cylinderVertexBuffer->AddVertexAttribute("position", 0, 3, 0);
	cylinderVertexBuffer->AddVertexAttribute("color", 1, 3, 3);

	auto& cylinderIndexBuffer = pcLinesCylinder->GetIndexBuffer();
	Generate::LineCylinderIndexes(cylinderIndexBuffer, 36);
	pcLinesCylinder->SetPosition({ 5.0f, 2.0f, 7.0f });
	scene->AddObject(pcLinesCylinder);
	AddObject("PCLinesCylinder", pcLinesCylinder);

	std::shared_ptr<GraphicsObject> pcLinesSphere1 =
		std::make_shared<GraphicsObject>();
	pcLinesSphere1->SetGenerator(
		std::make_shared<PCSphereGenerator>(pcLinesSphere1));
	auto params = std::make_shared<PCSphereParams>();
	params->radius = 2.0f;
	params->slices = 10;
	params->stacks = 10;
	pcLinesSphere1->GetMaterial().color = { 0.0f, 1.0f, 0.0f };
	pcLinesSphere1->GetGenerator()->SetParameters(params);
	pcLinesSphere1->Generate(UseDynamicBuffers);
	pcLinesSphere1->SetPosition({ -5.0f, 2.0f, 15.0f });
	pcLinesSphere1->CreateBoundingSphere(2.0f);
	auto ccb = std::make_shared<ChangeColorBehavior>();
	ccb->SetObject(pcLinesSphere1);
	pcLinesSphere1->AddBehavior("changecolor", ccb);
	scene->AddObject(pcLinesSphere1);
	AddObject("PCLinesSphere1", pcLinesSphere1);

	std::shared_ptr<GraphicsObject> pcLinesSphere2 =
		std::make_shared<GraphicsObject>();
	pcLinesSphere2->SetGenerator(
		std::make_shared<PCSphereGenerator>(pcLinesSphere2));
	params = std::make_shared<PCSphereParams>();
	params->radius = 2.0f;
	params->slices = 10;
	params->stacks = 10;
	pcLinesSphere2->GetMaterial().color = { 0.0f, 1.0f, 0.0f };
	pcLinesSphere2->GetGenerator()->SetParameters(params);
	pcLinesSphere2->Generate(UseDynamicBuffers);
	pcLinesSphere2->SetPosition({ 5.0f, 2.0f, 15.0f });
	pcLinesSphere2->CreateBoundingSphere(2.0f);
	ccb = std::make_shared<ChangeColorBehavior>(glm::vec3(0.0f, 0.0f, 1.0f));
	ccb->SetObject(pcLinesSphere2);
	pcLinesSphere2->AddBehavior("changecolor", ccb);

	scene->AddObject(pcLinesSphere2);
	AddObject("PCLinesSphere2", pcLinesSphere2);

	std::shared_ptr<GraphicsObject> pcLineCuboid =
		std::make_shared<GraphicsObject>();
	pcLineCuboid->SetGenerator(
		std::make_shared<PCCuboidGenerator>(pcLineCuboid));
	auto cparams = std::make_shared<PCCuboidParams>();
	cparams->width = 2.0f;
	cparams->height = 2.0f;
	cparams->depth = 2.0f;
	pcLineCuboid->GetMaterial().color = { 1.0f, 0.0f, 0.0f };
	pcLineCuboid->GetGenerator()->SetParameters(cparams);
	pcLineCuboid->Generate(UseDynamicBuffers);
	pcLineCuboid->SetPosition({ -5.0f, 2.0f, 20.0f });
	pcLineCuboid->CreateBoundingBox(2.0f, 2.0f, 2.0f);
	ccb = std::make_shared<ChangeColorBehavior>(glm::vec3(1.0f, 0.0f, 1.0f));
	ccb->SetObject(pcLineCuboid);
	pcLineCuboid->AddBehavior("changecolor", ccb);
	auto rb = std::make_shared<RotateBehavior>(pcLineCuboid);
	pcLineCuboid->AddBehavior("rotateY", rb);
	scene->AddObject(pcLineCuboid);
	AddObject("PCLineCuboid1", pcLineCuboid);

	std::shared_ptr<GraphicsObject> pcLineCuboid2 =
		std::make_shared<GraphicsObject>();
	pcLineCuboid2->SetGenerator(
		std::make_shared<PCCuboidGenerator>(pcLineCuboid2));
	cparams = std::make_shared<PCCuboidParams>();
	cparams->width = 2.0f;
	cparams->height = 2.0f;
	cparams->depth = 2.0f;
	pcLineCuboid2->GetMaterial().color = { 1.0f, 0.0f, 0.0f };
	pcLineCuboid2->GetGenerator()->SetParameters(cparams);
	pcLineCuboid2->Generate(UseDynamicBuffers);
	pcLineCuboid2->SetPosition({ 5.0f, 2.0f, 20.0f });
	pcLineCuboid2->CreateBoundingBox(2.0f, 2.0f, 2.0f);
	ccb = std::make_shared<ChangeColorBehavior>(glm::vec3(1.0f, 1.0f, 0.0f));
	ccb->SetObject(pcLineCuboid2);
	pcLineCuboid2->AddBehavior("changecolor", ccb);
	rb = std::make_shared<RotateBehavior>(pcLineCuboid2);
	pcLineCuboid2->AddBehavior("rotateY", rb);
	scene->AddObject(pcLineCuboid2);
	AddObject("PCLineCuboid2", pcLineCuboid2);

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

}

void GraphicsWorld::CreateRenderers()
{
	CreateRenderer1();
	CreateRenderer2();
	CreateRenderer3();
}

void GraphicsWorld::CreateRenderer1()
{
	_env->CreateRenderer("Lighting", _env->GetShader("Lighting"));
	_env->GetRenderer("Lighting")->SetScene(_env->GetScene("Lighting"));
}

void GraphicsWorld::CreateRenderer2()
{
	_env->CreateRenderer("BasicTexture", _env->GetShader("BasicTexture"));
	_env->GetRenderer("BasicTexture")->SetScene(_env->GetScene("BasicTexture"));
}

void GraphicsWorld::CreateRenderer3()
{
	_env->CreateRenderer("Basic", _env->GetShader("Basic"));
	_env->GetRenderer("Basic")->SetScene(_env->GetScene("Basic"));
}

void GraphicsWorld::OnMouseButton(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		auto worldCube = _env->GetGraphicsObject("World");
		if (worldCube->IsIntersectingWithRay(mouseRay)) {
			auto animation =
				std::dynamic_pointer_cast<MoveAnimation>(
					worldCube->GetAnimation());
			if (animation->GetState() == MoveAnimation::NOT_MOVING) {
				animation->SetState(MoveAnimation::MOVING);
			}
			else {
				animation->SetState(MoveAnimation::NOT_MOVING);
			}
		}
	}
}

void GraphicsWorld::OnKey(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS) {
		lookWithMouse = !lookWithMouse;
		return;
	}
}

void GraphicsWorld::PollInputs(float elapsedSeconds)
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


