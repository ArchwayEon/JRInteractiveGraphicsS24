#include <Windows.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VertexBuffer.h"
#include "GraphicsObject.h"
#include "Scene.h"
#include "Shader.h"
#include "Renderer.h"
#include "TextFile.h"
#include "GraphicsEnvironment.h"
#include "Generate.h"
#include "HighlightBehavior.h"
#include "PCSphereGenerator.h"
#include "ChangeColorBehavior.h"
#include "PCCuboidGenerator.h"
#include "RotateBehavior.h"

static void SetUpTexturedScene(
	std::shared_ptr<Shader>& textureShader, 
	std::shared_ptr<Scene>& textureScene)
{
	TextFile textFile;
	bool chk;
	chk = textFile.Read("texture.vert.glsl");
	if (chk == false) return;
	std::string vs = textFile.GetData();
	chk = textFile.Read("texture.frag.glsl");
	if (chk == false) return;
	std::string fs = textFile.GetData();
	textureShader = std::make_shared<Shader>(vs, fs);
	textureShader->AddUniform("projection");
	textureShader->AddUniform("world");
	textureShader->AddUniform("view");
	textureShader->AddUniform("texUnit");

	std::shared_ptr<Texture> rgbwTexture = std::make_shared<Texture>();
	rgbwTexture->SetDimension(4, 4);
	rgbwTexture->SetWrapS(GL_CLAMP_TO_EDGE);
	rgbwTexture->SetWrapT(GL_CLAMP_TO_EDGE);
	rgbwTexture->SetMagFilter(GL_LINEAR);
	unsigned char data[] = {
		255, 255, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 255, 255, 255, 255,
		0, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 0, 255,
		0, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 0, 255,
		255, 255, 255, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 255, 255, 255
	};
	rgbwTexture->SetTextureData(64, data);

	textureScene = std::make_shared<Scene>();

	std::shared_ptr<GraphicsObject> texturedSquare1 =
		std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> texturedBuffer1 =
		std::make_shared<VertexBuffer>(8);
	texturedBuffer1->AddVertexData(8,-20.0f, 20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 3.0f);
	texturedBuffer1->AddVertexData(8,-20.0f,-20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	texturedBuffer1->AddVertexData(8, 20.0f,-20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 3.0f, 0.0f);
	texturedBuffer1->AddVertexData(8,-20.0f, 20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 3.0f);
	texturedBuffer1->AddVertexData(8, 20.0f,-20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 3.0f, 0.0f);
	texturedBuffer1->AddVertexData(8, 20.0f, 20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 3.0f, 3.0f);
	texturedBuffer1->AddVertexAttribute("position", 0, 3, 0);
	texturedBuffer1->AddVertexAttribute("vertexColor", 1, 3, 3);
	texturedBuffer1->AddVertexAttribute("texCoord", 2, 2, 6);
	texturedBuffer1->SetTexture(rgbwTexture);
	texturedSquare1->SetVertexBuffer(texturedBuffer1);
	texturedSquare1->SetPosition(glm::vec3(-40, -20, 0));
	textureScene->AddObject(texturedSquare1);

	std::shared_ptr<Texture> pngTexture = std::make_shared<Texture>();
	pngTexture->LoadTextureDataFromFile("stairsAged_S.png");
	std::shared_ptr<GraphicsObject> texturedRect =
		std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> texturedBuffer2 =
		std::make_shared<VertexBuffer>(8);
	texturedBuffer2->AddVertexData(8, -12.0f, 25.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	texturedBuffer2->AddVertexData(8, -12.0f, -25.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	texturedBuffer2->AddVertexData(8, 13.0f, -25.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	texturedBuffer2->AddVertexData(8, -12.0f, 25.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	texturedBuffer2->AddVertexData(8, 13.0f, -25.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	texturedBuffer2->AddVertexData(8, 13.0f, 25.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	texturedBuffer2->AddVertexAttribute("position", 0, 3, 0);
	texturedBuffer2->AddVertexAttribute("vertexColor", 1, 3, 3);
	texturedBuffer2->AddVertexAttribute("texCoord", 2, 2, 6);
	texturedBuffer2->SetTexture(pngTexture);
	texturedRect->SetVertexBuffer(texturedBuffer2);
	texturedRect->SetPosition(glm::vec3(40, 20, 0));
	textureScene->AddObject(texturedRect);
}

static void SetUpScene1(
	std::shared_ptr<Shader>& shader,
	std::shared_ptr<Scene>& scene)
{
	bool success;
	std::string vertexSource, fragmentSource;
	TextFile file;
	success = file.Read("basic.vert.glsl");
	vertexSource = file.GetData();
	success = file.Read("basic.frag.glsl");
	fragmentSource = file.GetData();
	if (success == false) return;

	shader = std::make_shared<Shader>(vertexSource, fragmentSource);
	shader->AddUniform("projection");
	shader->AddUniform("world");
	shader->AddUniform("view");

	scene = std::make_shared<Scene>();
	std::shared_ptr<GraphicsObject> square = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> buffer = std::make_shared<VertexBuffer>(6);
	buffer->AddVertexData(6, -5.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	buffer->AddVertexData(6, -5.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	buffer->AddVertexData(6, 5.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	buffer->AddVertexData(6, -5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	buffer->AddVertexData(6, 5.0f, -5.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	buffer->AddVertexData(6, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	buffer->AddVertexAttribute("position", 0, 3);
	buffer->AddVertexAttribute("color", 1, 3, 3);
	square->SetVertexBuffer(buffer);
	scene->AddObject(square);

	std::shared_ptr<GraphicsObject> triangle = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> buffer2 = std::make_shared<VertexBuffer>(6);
	buffer2->AddVertexData(6, -5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	buffer2->AddVertexData(6, -5.0f, -5.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	buffer2->AddVertexData(6, 5.0f, -5.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	buffer2->AddVertexAttribute("position", 0, 3);
	buffer2->AddVertexAttribute("color", 1, 3, 3);
	triangle->SetVertexBuffer(buffer2);
	triangle->SetPosition(glm::vec3(30.0f, 0.0f, 0.0f));
	scene->AddObject(triangle);

	std::shared_ptr<GraphicsObject> line = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> buffer3 = std::make_shared<VertexBuffer>(6);
	buffer3->SetPrimitiveType(GL_LINES);
	buffer3->AddVertexData(6, 0.0f,  2.5f, 0.0f, 0.0f, 1.0f, 0.0f);
	buffer3->AddVertexData(6, 0.0f, -2.5f, 0.0f, 0.0f, 1.0f, 0.0f);
	buffer3->AddVertexAttribute("position", 0, 3);
	buffer3->AddVertexAttribute("color", 1, 3, 3);
	line->SetVertexBuffer(buffer3);
	line->SetPosition(glm::vec3(5.0f, -10.0f, 0.0f));
	triangle->AddChild(line);

}

static void SetUp3DScene1(
	std::shared_ptr<Shader>& shader, std::shared_ptr<Scene>& scene, GraphicsEnvironment& env)
{
	TextFile textFile;
	bool chk;
	chk = textFile.Read("texture.vert.glsl");
	if (chk == false) return;
	std::string vs = textFile.GetData();
	chk = textFile.Read("texture.frag.glsl");
	if (chk == false) return;
	std::string fs = textFile.GetData();
	shader = std::make_shared<Shader>(vs, fs);
	shader->AddUniform("projection");
	shader->AddUniform("world");
	shader->AddUniform("view");
	shader->AddUniform("texUnit");

	std::shared_ptr<Texture> rgbwTexture = std::make_shared<Texture>();
	rgbwTexture->SetDimension(4, 4);
	unsigned char data[] = {
		255, 255, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 255, 255, 255, 255,
		0, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 0, 255,
		0, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 0, 255,
		255, 255, 255, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 255, 255, 255
	};
	rgbwTexture->SetTextureData(64, data);

	scene = std::make_shared<Scene>();

	std::shared_ptr<GraphicsObject> texturedCube = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> buffer = Generate::Cuboid(10.0f, 5.0f, 5.0f);
	buffer->AddVertexAttribute("position", 0, 3, 0);
	buffer->AddVertexAttribute("vertexColor", 1, 3, 3);
	buffer->AddVertexAttribute("texCoord", 2, 2, 6);
	buffer->SetTexture(rgbwTexture);
	texturedCube->SetVertexBuffer(buffer);
	texturedCube->CreateBoundingBox(10.0f, 5.0f, 5.0f);
	scene->AddObject(texturedCube);
	env.AddObject("TexturedCube", texturedCube);

	std::shared_ptr<Texture> crateTexture = std::make_shared<Texture>();
	crateTexture->LoadTextureDataFromFile("crate.jpg");
	std::shared_ptr<GraphicsObject> crate = std::make_shared<GraphicsObject>();
	buffer = Generate::Cuboid(10.0f, 10.0f, 10.0f);
	buffer->AddVertexAttribute("position", 0, 3, 0);
	buffer->AddVertexAttribute("vertexColor", 1, 3, 3);
	buffer->AddVertexAttribute("texCoord", 2, 2, 6);
	buffer->SetTexture(crateTexture);
	crate->SetVertexBuffer(buffer);
	crate->CreateBoundingBox(10.0f, 10.0f, 10.0f);
	crate->SetPosition(glm::vec3(-20.0f, 5.0f, -5.0f));
	scene->AddObject(crate);
	env.AddObject("Crate", crate);

	std::shared_ptr<Texture> floorTexture = std::make_shared<Texture>();
	floorTexture->LoadTextureDataFromFile("floor.jpg");
	std::shared_ptr<GraphicsObject> floor = std::make_shared<GraphicsObject>();
	buffer = Generate::XZPlane(50.0f, 50.0f, {1.0f, 1.0f, 1.0f}, {10.0f, 10.0f});
	buffer->AddVertexAttribute("position", 0, 3, 0);
	buffer->AddVertexAttribute("vertexColor", 1, 3, 3);
	buffer->AddVertexAttribute("texCoord", 2, 2, 6);
	buffer->SetTexture(floorTexture);
	floor->SetVertexBuffer(buffer);
	floor->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	scene->AddObject(floor);
	env.AddObject("Floor", floor);

}

static void SetUp3DScene2(
	std::shared_ptr<Shader>& shader, std::shared_ptr<Scene>& scene, GraphicsEnvironment& env)
{
	TextFile textFile;
	bool chk;
	chk = textFile.Read("lighting.vert.glsl");
	if (chk == false) return;
	std::string vs = textFile.GetData();
	chk = textFile.Read("lighting.frag.glsl");
	if (chk == false) return;
	std::string fs = textFile.GetData();
	shader = std::make_shared<Shader>(vs, fs);
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

	std::shared_ptr<Texture> rgbwTexture = std::make_shared<Texture>();
	rgbwTexture->SetDimension(4, 4);
	unsigned char data[] = {
		255, 255, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 255, 255, 255, 255,
		0, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 0, 255,
		0, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 0, 255,
		255, 255, 255, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 255, 255, 255
	};
	rgbwTexture->SetTextureData(64, data);

	scene = std::make_shared<Scene>();

	std::shared_ptr<GraphicsObject> texturedCube = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> buffer = Generate::CuboidWithNormals(10.0f, 5.0f, 5.0f);
	buffer->AddVertexAttribute("position", 0, 3, 0);
	buffer->AddVertexAttribute("vertexColor", 1, 4, 3);
	buffer->AddVertexAttribute("vertexNormal", 2, 3, 7);
	buffer->AddVertexAttribute("texCoord", 3, 2, 10);
	buffer->SetTexture(rgbwTexture);
	texturedCube->SetVertexBuffer(buffer);
	texturedCube->CreateBoundingBox(10.0f, 5.0f, 5.0f);
	auto hb1 = std::make_shared<HighlightBehavior>();
	hb1->SetObject(texturedCube);
	texturedCube->AddBehavior("highlight", hb1);
	scene->AddObject(texturedCube);
	env.AddObject("TexturedCube", texturedCube);

	std::shared_ptr<Texture> crateTexture = std::make_shared<Texture>();
	crateTexture->LoadTextureDataFromFile("crate.jpg");
	std::shared_ptr<GraphicsObject> crate = std::make_shared<GraphicsObject>();
	buffer = Generate::CuboidWithNormals(10.0f, 10.0f, 10.0f);
	buffer->AddVertexAttribute("position", 0, 3, 0);
	buffer->AddVertexAttribute("vertexColor", 1, 4, 3);
	buffer->AddVertexAttribute("vertexNormal", 2, 3, 7);
	buffer->AddVertexAttribute("texCoord", 3, 2, 10);
	buffer->SetTexture(crateTexture);
	crate->SetVertexBuffer(buffer);
	crate->CreateBoundingBox(10.0f, 10.0f, 10.0f);
	auto hb2 = std::make_shared<HighlightBehavior>();
	hb2->SetObject(crate);
	crate->AddBehavior("highlight", hb2);
	crate->SetPosition(glm::vec3(-20.0f, 5.0f, -5.0f));
	scene->AddObject(crate);
	env.AddObject("Crate", crate);

	std::shared_ptr<Texture> worldTexture = std::make_shared<Texture>();
	worldTexture->LoadTextureDataFromFile("world.jpg");
	std::shared_ptr<GraphicsObject> world = std::make_shared<GraphicsObject>();
	buffer = Generate::CuboidWithNormals(3.0f, 3.0f, 3.0f);
	buffer->AddVertexAttribute("position", 0, 3, 0);
	buffer->AddVertexAttribute("vertexColor", 1, 4, 3);
	buffer->AddVertexAttribute("vertexNormal", 2, 3, 7);
	buffer->AddVertexAttribute("texCoord", 3, 2, 10);
	buffer->SetTexture(worldTexture);
	world->SetVertexBuffer(buffer);
	world->CreateBoundingBox(3.0f, 3.0f, 3.0f);
	auto hb3 = std::make_shared<HighlightBehavior>();
	hb3->SetObject(world);
	world->AddBehavior("highlight", hb3);
	world->SetPosition(glm::vec3(20.0f, 1.5f, 22.0f));
	scene->AddObject(world);
	env.AddObject("World", world);

	std::shared_ptr<Texture> floorTexture = std::make_shared<Texture>();
	floorTexture->LoadTextureDataFromFile("floor.jpg");
	std::shared_ptr<GraphicsObject> floor = std::make_shared<GraphicsObject>();
	buffer = Generate::XZPlaneWithNormals(50.0f, 50.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, { 10.0f, 10.0f });
	buffer->AddVertexAttribute("position", 0, 3, 0);
	buffer->AddVertexAttribute("vertexColor", 1, 4, 3);
	buffer->AddVertexAttribute("vertexNormal", 2, 3, 7);
	buffer->AddVertexAttribute("texCoord", 3, 2, 10);
	buffer->SetTexture(floorTexture);
	floor->SetVertexBuffer(buffer);
	floor->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	scene->AddObject(floor);
	env.AddObject("Floor", floor);

	scene->GetLocalLight().position = { 0.0f, 5.0f, 5.0f };
	scene->GetLocalLight().intensity = 0.5f;
}

static void SetUpLightBulbScene(
	std::shared_ptr<Shader>& shader, std::shared_ptr<Scene>& scene, 
	GraphicsEnvironment& env)
{
	TextFile textFile;
	bool chk;
	chk = textFile.Read("texture.vert.glsl");
	if (chk == false) return;
	std::string vs = textFile.GetData();
	chk = textFile.Read("texture.frag.glsl");
	if (chk == false) return;
	std::string fs = textFile.GetData();
	shader = std::make_shared<Shader>(vs, fs);
	shader->AddUniform("projection");
	shader->AddUniform("world");
	shader->AddUniform("view");
	shader->AddUniform("texUnit");
	scene = std::make_shared<Scene>();

	std::shared_ptr<Texture> lightBulbTexture = std::make_shared<Texture>();
	lightBulbTexture->LoadTextureDataFromFile("lightbulb.png");
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
	env.AddObject("LightBulb", lightBulb);
}

static void SetUpPCObjectsScene(
	std::shared_ptr<Shader>& shader, std::shared_ptr<Scene>& scene,
	GraphicsEnvironment& env) 
{
	TextFile textFile;
	bool chk;
	chk = textFile.Read("basic.vert.glsl");
	if (chk == false) return;
	std::string vs = textFile.GetData();
	chk = textFile.Read("basic.frag.glsl");
	if (chk == false) return;
	std::string fs = textFile.GetData();
	shader = std::make_shared<Shader>(vs, fs);
	shader->AddUniform("projection");
	shader->AddUniform("world");
	shader->AddUniform("view");

	scene = std::make_shared<Scene>();

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
	env.AddObject("PCLinesCircle", pcLinesCircle);

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
	env.AddObject("PCLinesCylinder", pcLinesCylinder);

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
	env.AddObject("PCLinesSphere1", pcLinesSphere1);

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
	env.AddObject("PCLinesSphere2", pcLinesSphere2);

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
	pcLineCuboid->SetPosition({-5.0f, 2.0f, 20.0f });
	pcLineCuboid->CreateBoundingBox(2.0f, 2.0f, 2.0f);
	ccb = std::make_shared<ChangeColorBehavior>(glm::vec3(1.0f, 0.0f, 1.0f));
	ccb->SetObject(pcLineCuboid);
	pcLineCuboid->AddBehavior("changecolor", ccb);
	auto rb = std::make_shared<RotateBehavior>(pcLineCuboid);
	pcLineCuboid->AddBehavior("rotateY", rb);
	scene->AddObject(pcLineCuboid);
	env.AddObject("PCLineCuboid1", pcLineCuboid);

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
	env.AddObject("PCLineCuboid2", pcLineCuboid2);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	GraphicsEnvironment glfw;
	glfw.Init(4, 3);

	bool created = glfw.SetWindow(
		1200, 800, "ETSU Computing Interactive Graphics");
	if (created == false) return -1;

	bool loaded = glfw.InitGlad();
	if (loaded == false) return -1;

	glfw.SetupGraphics();

	std::shared_ptr<Shader> shader;
	std::shared_ptr<Scene> scene;
	SetUp3DScene2(shader, scene, glfw);

	std::shared_ptr<Shader> basicShader;
	std::shared_ptr<Scene> basicScene;
	SetUpLightBulbScene(basicShader, basicScene, glfw);

	std::shared_ptr<Shader> pcShader;
	std::shared_ptr<Scene> pcScene;
	SetUpPCObjectsScene(pcShader, pcScene, glfw);

	glfw.CreateRenderer("LightingRenderer", shader);
	glfw.GetRenderer("LightingRenderer")->SetScene(scene);

	glfw.CreateRenderer("BasicRenderer", basicShader);
	glfw.GetRenderer("BasicRenderer")->SetScene(basicScene);

	glfw.CreateRenderer("PCRenderer", pcShader);
	glfw.GetRenderer("PCRenderer")->SetScene(pcScene);

	glfw.Allocate();

	glfw.Run3D();
	return 0;
}

