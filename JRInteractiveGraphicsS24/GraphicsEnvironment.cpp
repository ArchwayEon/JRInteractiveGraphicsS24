#include "GraphicsEnvironment.h"
#include <glm/gtc/matrix_transform.hpp>

GraphicsEnvironment::GraphicsEnvironment() : window(nullptr)
{
}

GraphicsEnvironment::~GraphicsEnvironment()
{
    glfwTerminate();
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
        _log << "Failed to create GLFW window" << std::endl;
        return false;
    }
    glfwMakeContextCurrent(window);
    return true;
}

bool GraphicsEnvironment::InitGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        _log << "Failed to initialize GLAD" << std::endl;
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
    //glfwMaximizeWindow(window);

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

std::shared_ptr<Renderer> GraphicsEnvironment::GetRenderer(const std::string& name)
{
    return rendererMap[name];
}

void GraphicsEnvironment::StaticAllocate()
{
    for (const auto& [key, renderer] : rendererMap) {
        renderer->StaticAllocateVertexBuffers();
    }
}

void GraphicsEnvironment::Render()
{
    for (const auto& [key, renderer] : rendererMap) {
        renderer->RenderScene();
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
		ProcessInput();
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
		ImGui::Text(GetLog().c_str());
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
    float cubeYAngle = 0;
    float cubeXAngle = 0;
    float cubeZAngle = 0;
    float left = -20.0f;
    float right = 20.0f;
    float bottom = -20.0f;
    float top = 20.0f;
    int width, height;

    float aspectRatio;
    float nearPlane = 1.0f;
    float farPlane = 100.0f;
    float fieldOfView = 60;

    glm::vec3 cameraPosition(15.0f, 15.0f, 20.0f);
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 referenceFrame(1.0f);
    glm::vec3 clearColor = { 0.0f, 0.0f, 0.0f };

    auto& scene = GetRenderer("renderer")->GetScene();
    auto& cube = scene->GetObjects()[0];

    ImGuiIO& io = ImGui::GetIO();
    while (!glfwWindowShouldClose(window)) {
        ProcessInput();
        glfwGetWindowSize(window, &width, &height);

        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        referenceFrame = glm::rotate(glm::mat4(1.0f), glm::radians(cubeYAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        referenceFrame = glm::rotate(referenceFrame, glm::radians(cubeXAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        referenceFrame = glm::rotate(referenceFrame, glm::radians(cubeZAngle), glm::vec3(0.0f, 0.0f, 1.0f));

        view = glm::lookAt(cameraPosition, cameraTarget, cameraUp);

        if (width >= height) {
            aspectRatio = width / (height * 1.0f);
        }
        else {
            aspectRatio = height / (width * 1.0f);
        }
        projection = glm::perspective(
            glm::radians(fieldOfView), aspectRatio, nearPlane, farPlane);

        SetRendererProjectionAndView(projection, view);
        
        cube->SetReferenceFrame(referenceFrame);

        Render();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Computing Interactive Graphics");
        ImGui::Text(GetLog().c_str());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / io.Framerate, io.Framerate);
        ImGui::ColorEdit3("Background color", (float*)&clearColor.r);
        ImGui::SliderFloat("X Angle", &cubeXAngle, 0, 360);
        ImGui::SliderFloat("Y Angle", &cubeYAngle, 0, 360);
        ImGui::SliderFloat("Z Angle", &cubeZAngle, 0, 360);
        ImGui::SliderFloat("Camera X", &cameraPosition.x, left, right);
        ImGui::SliderFloat("Camera Y", &cameraPosition.y, bottom, top);
        ImGui::SliderFloat("Camera Z", &cameraPosition.z, 20, 50);
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ShutDown();
}

void GraphicsEnvironment::SetRendererProjectionAndView(const glm::mat4& projection, const glm::mat4& view)
{
	for (const auto& [key, renderer] : rendererMap) {
		renderer->SetProjection(projection);
		renderer->SetView(view);
	}
}


void GraphicsEnvironment::OnWindowSizeChanged(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void GraphicsEnvironment::ProcessInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

glm::mat4 GraphicsEnvironment::CreateViewMatrix(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up)
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
