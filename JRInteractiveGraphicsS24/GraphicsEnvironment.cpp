#include <Windows.h>
#include "GraphicsEnvironment.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Timer.h"
#include "RotateAnimation.h"
#include "GeometricPlane.h"
#include "MoveAnimation.h"
#include "Logger.h"

GraphicsEnvironment* GraphicsEnvironment::self;

GraphicsEnvironment::GraphicsEnvironment() : 
    window(nullptr), projection(1.0f), view(1.0f)
{
    currentCamera = std::make_shared<Camera>();
    cameraMap["Current"] = currentCamera;
    objectManager = std::make_shared<ObjectManager>();
    self = this;
}

GraphicsEnvironment::~GraphicsEnvironment()
{
    glfwTerminate();
}

void GraphicsEnvironment::ShowNotice(const std::string& message)
{
    // Quick way to convert from string to wstring
    std::wstring messageStr(message.begin(), message.end());
    MessageBox(
        nullptr,
        messageStr.c_str(),
        L"Notice",
        MB_OK);
}

void GraphicsEnvironment::Init(
    unsigned int majorVersion, unsigned int minorVersion)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
}

bool GraphicsEnvironment::SetWindow(unsigned int width, unsigned int height, const std::string& title)
{
    window = glfwCreateWindow(
        width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        Logger::Log("Failed to create GLFW window!");
        return false;
    }
    glfwMakeContextCurrent(window);
    return true;
}

bool GraphicsEnvironment::InitGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Logger::Log("Failed to initialize GLAD!");
        return false;
    }
    return true;
}

void GraphicsEnvironment::SetupGraphics()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Cull back faces and use counter-clockwise winding of front faces
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    glEnable(GL_MULTISAMPLE);

    glViewport(0, 0, 1200, 800);
    glfwSetFramebufferSizeCallback(window, OnWindowSizeChanged);
    //glfwSetCursorPosCallback(window, OnMouseMove);
    glfwSetMouseButtonCallback(window, OnMouseButton);
    glfwSetKeyCallback(window, OnKey);
    glfwMaximizeWindow(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
}

void GraphicsEnvironment::CreateRenderer(
    const std::string& name, std::shared_ptr<Shader> shader)
{
    auto renderer = std::make_shared<Renderer>(shader);
    rendererMap.emplace(name, renderer);
}

std::shared_ptr<Renderer> GraphicsEnvironment::GetRenderer(
    const std::string& name)
{
    return rendererMap[name];
}

void GraphicsEnvironment::Allocate()
{
    for (const auto& [key, renderer] : rendererMap) {
        renderer->AllocateBuffers();
    }
}

void GraphicsEnvironment::Render()
{
    for (const auto& [key, renderer] : rendererMap) {
        renderer->RenderScene(currentCamera);
    }
}

void GraphicsEnvironment::Run2D()
{
	int width, height;
	float aspectRatio;

	float left = -50.0f;
	float right = 50.0f;
	float bottom = -50.0f;
	float top = 50.0f;
	glm::mat4 projection;

	glm::vec3 clearColor = { 0.2f, 0.3f, 0.3f };
	float angle = 0, childAngle = 0;
	float cameraX = -10, cameraY = 0;
	glm::mat4 view;

	ImGuiIO& io = ImGui::GetIO();

	while (!glfwWindowShouldClose(window)) {
		PollInputs(0);
		glfwGetWindowSize(window, &width, &height);

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		view = CreateViewMatrix(
			glm::vec3(cameraX, cameraY, 1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		if (width >= height) {
			aspectRatio = width / (height * 1.0f);
			projection = glm::ortho(
				left * aspectRatio, right * aspectRatio, bottom, top, -1.0f, 1.0f);
		}
		else {
			aspectRatio = height / (width * 1.0f);
			projection = glm::ortho(
				left, right, bottom * aspectRatio, top * aspectRatio, -1.0f, 1.0f);
		}
		SetRendererProjectionAndView(projection, view);

		GetRenderer("renderer")->GetScene()->Update(angle, childAngle);

		Render();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Computing Interactive Graphics");
		ImGui::Text(Logger::GetLog().c_str());
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / io.Framerate, io.Framerate);
		ImGui::ColorEdit3("Background color", (float*)&clearColor.r);
		ImGui::SliderFloat("Angle", &angle, 0, 360);
		ImGui::SliderFloat("Child Angle", &childAngle, 0, 360);
		ImGui::SliderFloat("Camera X", &cameraX, left, right);
		ImGui::SliderFloat("Camera Y", &cameraY, bottom, top);

		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    ShutDown();
}

void GraphicsEnvironment::ShutDown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GraphicsEnvironment::Run3D()
{
    glm::vec3 clearColor = { 0.0f, 0.0f, 0.0f };
    double elapsedSeconds;
    Timer timer;
    ImGuiIO& io = ImGui::GetIO();
    currentWorld->SetCamera(currentCamera);
    currentWorld->Preupdate();
    while (!glfwWindowShouldClose(window)) {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        elapsedSeconds = timer.GetElapsedTimeInSeconds();
        currentWorld->Update((float)elapsedSeconds);

        objectManager->Update(elapsedSeconds);

        Render();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Computing Interactive Graphics");
        ImGui::ColorEdit3("Background color", (float*)&clearColor.r);
        currentWorld->UI(io);
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ShutDown();
}

void GraphicsEnvironment::SetRendererProjectionAndView(
    const glm::mat4& projection, const glm::mat4& view)
{
	for (const auto& [key, renderer] : rendererMap) {
		renderer->SetProjection(projection);
		renderer->SetView(view);
	}
}

void GraphicsEnvironment::AddObject(
    const std::string& name, std::shared_ptr<GraphicsObject> object)
{
    object->SetName(name);
    objectManager->AddObject(name, object);
}


Ray GraphicsEnvironment::GetMouseRay(
    const glm::mat4& projection, const glm::mat4& view)
{
    Ray ray;
    auto projInverse = glm::inverse(projection);
    auto viewInverse = glm::inverse(view);

    glm::vec4 rayStartClip = { mouse.nsx, mouse.nsy, 0, 0 };
    glm::vec4 rayStartEye = projInverse * rayStartClip;
    rayStartEye.z = 1.0f;
    rayStartEye.w = 1.0f; // This is a position vector
    glm::vec3 start = glm::vec3(viewInverse * rayStartEye);

    glm::vec4 rayDirClip = { mouse.nsx, mouse.nsy, 0, 0 };
    glm::vec4 rayDirEye = projInverse * rayDirClip;
    rayDirEye.z = -1.0f; // Forward, down the -ve z
    rayDirEye.w = 0.0f; // This is a direction vector
    glm::vec3 dir = glm::vec3(viewInverse * rayDirEye);

    ray.SetStartPoint(start);
    ray.SetDirection(dir);
    return ray;
}

void GraphicsEnvironment::UpdateMousePosition()
{
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    mouse.x = mouseX;
    mouse.y = mouseY;

    float xPercent = static_cast<float>(mouse.x / mouse.windowWidth);
    float yPercent = static_cast<float>(mouse.y / mouse.windowHeight);

    mouse.spherical.theta = 90.0f - (xPercent * 180); // left/right
    mouse.spherical.phi = 180.0f - (yPercent * 180); // up/down

    mouse.nsx = xPercent * 2.0f - 1.0f;
    mouse.nsy = -(yPercent * 2.0f - 1.0f);
}

void GraphicsEnvironment::UpdateWindowSize()
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    mouse.windowWidth = width;
    mouse.windowHeight = height;
}

void GraphicsEnvironment::AddShader(
    const std::string& name, std::shared_ptr<Shader> shader)
{
    if (shaderMap.contains(name)) return;
    shaderMap[name] = shader;
}

void GraphicsEnvironment::AddScene(
    const std::string& name, std::shared_ptr<Scene> scene)
{
    if (sceneMap.contains(name)) return;
    sceneMap[name] = scene;
}

void GraphicsEnvironment::AddTexture(
    const std::string& name, std::shared_ptr<Texture> texture)
{
    if (textureMap.contains(name)) return;
    textureMap[name] = texture;
}

void GraphicsEnvironment::AddGraphicsWorld(
    const std::string& name, std::shared_ptr<IGraphicsWorld> world)
{
    if (worldMap.contains(name)) return;
    worldMap[name] = world;
}

void GraphicsEnvironment::CreateWorlds()
{
    for (const auto& [key, world] : worldMap) {
        world->Create();
    }
    Allocate();
}

void GraphicsEnvironment::OnWindowSizeChanged(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void GraphicsEnvironment::OnMouseMove(
    GLFWwindow* window, double mouseX, double mouseY)
{
    self->UpdateMousePosition();
}

void GraphicsEnvironment::OnMouseButton(
    GLFWwindow* window, int button, int action, int mods)
{
    self->currentWorld->OnMouseButton(button, action, mods);
}

void GraphicsEnvironment::OnKey(
    GLFWwindow* window, int key, int scancode, int action, int mods)
{
    self->currentWorld->OnKey(key, scancode, action, mods);
}

void GraphicsEnvironment::PollInputs(double elapsedSeconds)
{
    currentWorld->PollInputs((float)elapsedSeconds);
}

glm::mat4 GraphicsEnvironment::CreateViewMatrix(
    const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up)
{
    glm::vec3 right = glm::cross(direction, up);
    right = glm::normalize(right);

    glm::vec3 vUp = glm::cross(right, direction);
    vUp = glm::normalize(vUp);

    glm::mat4 view(1.0f);
    view[0] = glm::vec4(right, 0.0f);
    view[1] = glm::vec4(up, 0.0f);
    view[2] = glm::vec4(direction, 0.0f);
    view[3] = glm::vec4(position, 1.0f);
    return glm::inverse(view);
}
