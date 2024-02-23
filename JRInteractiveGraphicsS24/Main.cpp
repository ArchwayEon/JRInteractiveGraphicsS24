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
	std::shared_ptr<Shader>& shader, std::shared_ptr<Scene>& scene)
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
	scene->AddObject(texturedCube);

	std::shared_ptr<Texture> crateTexture = std::make_shared<Texture>();
	crateTexture->LoadTextureDataFromFile("crate.jpg");
	std::shared_ptr<GraphicsObject> crate = std::make_shared<GraphicsObject>();
	buffer = Generate::Cuboid(10.0f, 10.0f, 10.0f);
	buffer->AddVertexAttribute("position", 0, 3, 0);
	buffer->AddVertexAttribute("vertexColor", 1, 3, 3);
	buffer->AddVertexAttribute("texCoord", 2, 2, 6);
	buffer->SetTexture(crateTexture);
	crate->SetVertexBuffer(buffer);
	crate->SetPosition(glm::vec3(-20.0f, 5.0f, -5.0f));
	scene->AddObject(crate);

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
	SetUp3DScene1(shader, scene);

	glfw.CreateRenderer("renderer", shader);
	glfw.GetRenderer("renderer")->SetScene(scene);
	glfw.StaticAllocate();

	glfw.Run3D();
	return 0;
}

