#include "GraphicsWorld.h"
#include "GraphicsEnvironment.h"
#include "TextFile.h"
#include "Generate.h"
#include "HighlightBehavior.h"
#include "RotateBehavior.h"
#include "PCSphereGenerator.h"
#include "ChangeColorBehavior.h"
#include "PCCuboidGenerator.h"

GraphicsWorld::GraphicsWorld(std::shared_ptr<GraphicsEnvironment> env)
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

void GraphicsWorld::CreateScene1()
{
	TextFile textFile;
	bool chk;
	chk = textFile.Read("lighting.vert.glsl");
	if (chk == false) return;
	std::string vs = textFile.GetData();
	chk = textFile.Read("lighting.frag.glsl");
	if (chk == false) return;
	std::string fs = textFile.GetData();
	std::shared_ptr<Shader> shader = std::make_shared<Shader>(vs, fs);
	_env->AddShader("Lighting", shader);
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
	auto rb = std::make_shared<RotateBehavior>(texturedCube);
	rb->SetUpParameters();
	rb->GetParameter()->axis = { 0.0f, 1.0f, 0.0f };
	texturedCube->AddBehavior("rotateY", rb);
	rb = std::make_shared<RotateBehavior>(texturedCube);
	rb->SetUpParameters();
	rb->GetParameter()->axis = { 1.0f, 0.0f, 0.0f };
	texturedCube->AddBehavior("rotateX", rb);
	rb = std::make_shared<RotateBehavior>(texturedCube);
	rb->SetUpParameters();
	rb->GetParameter()->axis = { 0.0f, 0.0f, 1.0f };
	texturedCube->AddBehavior("rotateZ", rb);
	scene->AddObject(texturedCube);
	_env->AddObject("TexturedCube", texturedCube);

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
	_env->AddObject("Crate", crate);

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
	_env->AddObject("World", world);

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
	_env->AddObject("Floor", floor);

	scene->GetLocalLight().position = { 0.0f, 5.0f, 5.0f };
	scene->GetLocalLight().intensity = 0.5f;
}

void GraphicsWorld::CreateScene2()
{
	TextFile textFile;
	bool chk;
	chk = textFile.Read("texture.vert.glsl");
	if (chk == false) return;
	std::string vs = textFile.GetData();
	chk = textFile.Read("texture.frag.glsl");
	if (chk == false) return;
	std::string fs = textFile.GetData();
	auto shader = std::make_shared<Shader>(vs, fs);
	_env->AddShader("BasicTexture", shader);

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
	_env->AddObject("LightBulb", lightBulb);
}

void GraphicsWorld::CreateScene3()
{
	TextFile textFile;
	bool chk;
	chk = textFile.Read("basic.vert.glsl");
	if (chk == false) return;
	std::string vs = textFile.GetData();
	chk = textFile.Read("basic.frag.glsl");
	if (chk == false) return;
	std::string fs = textFile.GetData();

	auto shader = std::make_shared<Shader>(vs, fs);
	_env->AddShader("Basic", shader);

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
	_env->AddObject("PCLinesCircle", pcLinesCircle);

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
	_env->AddObject("PCLinesCylinder", pcLinesCylinder);

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
	_env->AddObject("PCLinesSphere1", pcLinesSphere1);

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
	_env->AddObject("PCLinesSphere2", pcLinesSphere2);

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
	_env->AddObject("PCLineCuboid1", pcLineCuboid);

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
	_env->AddObject("PCLineCuboid2", pcLineCuboid2);

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
	_env->AddObject("PCMouseRay", pcMouseRay);

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


