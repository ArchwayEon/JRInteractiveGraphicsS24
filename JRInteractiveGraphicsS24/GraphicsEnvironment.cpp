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
    objectManager = std::make_shared<ObjectManager>();
    self = this;
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
        renderer->RenderScene(camera);
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
		ProcessInput(0);
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

    camera.SetPosition(glm::vec3(0.0f, 5.0f, 30.0f));
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);

    glm::mat4 cubeReferenceFrame(1.0f);
    glm::vec3 clearColor = { 0.0f, 0.0f, 0.0f };
    
    auto lightBulb = objectManager->GetObject("LightBulb");

    std::shared_ptr<RotateAnimation> rotateAnimation = 
        std::make_shared<RotateAnimation>();
    rotateAnimation->SetObject(objectManager->GetObject("Crate"));
    objectManager->GetObject("Crate")->SetAnimation(rotateAnimation);

    std::shared_ptr<MoveAnimation> moveAnimation = 
        std::make_shared<MoveAnimation>();
    moveAnimation->SetObject(objectManager->GetObject("World"));
    objectManager->GetObject("World")->SetAnimation(moveAnimation);
    
    GeometricPlane floorPlane;
    float offset;
    double elapsedSeconds;
    Timer timer;
    ImGuiIO& io = ImGui::GetIO();
    auto mainScene = GetScene("Lighting");
    auto& globalLight = mainScene->GetGlobalLight();
    auto& localLight = mainScene->GetLocalLight();
    objectManager->SetBehaviorDefaults();
    lookWithMouse = false;
    bool isSphereOverlapping = false;
    glm::vec3 spherePos = 
        objectManager->GetObject("PCLinesSphere1")->GetPosition();
    bool isCuboidOverlapping = false;
    glm::vec3 cuboidPos =
        objectManager->GetObject("PCLineCuboid1")->GetPosition();
    float cuboid1Deg = 0.0f;
    float cuboid2Deg = 0.0f;

    auto cube = objectManager->GetObject("TexturedCube");
    auto sphere1 = objectManager->GetObject("PCLinesSphere1");
    auto sphere2 = objectManager->GetObject("PCLinesSphere2");
    auto cylinder = objectManager->GetObject("PCLinesCylinder");
    auto floor = objectManager->GetObject("Floor");
    auto cuboid1 = objectManager->GetObject("PCLineCuboid1");
    auto cuboid2 = objectManager->GetObject("PCLineCuboid2");
    auto crate = objectManager->GetObject("Crate");
    auto cubeWorld = objectManager->GetObject("World");
    auto mouseRayLine = objectManager->GetObject("PCMouseRay");
    while (!glfwWindowShouldClose(window)) {
        elapsedSeconds = timer.GetElapsedTimeInSeconds();
        ProcessInput(elapsedSeconds);
        glfwGetWindowSize(window, &width, &height);
        mouse.windowWidth = width;
        mouse.windowHeight = height;
        UpdateMousePosition();

        if (correctGamma) {
            glEnable(GL_FRAMEBUFFER_SRGB);
        }
        else {
            glDisable(GL_FRAMEBUFFER_SRGB);
        }

        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        cubeReferenceFrame = 
            glm::rotate(
                glm::mat4(1.0f), glm::radians(cubeYAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        cubeReferenceFrame = 
            glm::rotate(
                cubeReferenceFrame, glm::radians(cubeXAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        cubeReferenceFrame = 
            glm::rotate(
                cubeReferenceFrame, glm::radians(cubeZAngle), glm::vec3(0.0f, 0.0f, 1.0f));

        if (lookWithMouse) {
            camera.SetLookFrame(mouse.spherical.ToMat4());
        }

        view = camera.GetLookForwardViewMatrix();

        if (width >= height) {
            aspectRatio = width / (height * 1.0f);
        }
        else {
            aspectRatio = height / (width * 1.0f);
        }
        projection = glm::perspective(
            glm::radians(fieldOfView), aspectRatio, nearPlane, farPlane);

        SetRendererProjectionAndView(projection, view);
        mouseRay = GetMouseRay(projection, view);
        
        cube->SetReferenceFrame(cubeReferenceFrame);
        lightBulb->SetPosition(localLight.position);
        lightBulb->PointAt(camera.GetPosition());

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

        std::shared_ptr<HighlightParams> hp =
            std::make_shared<HighlightParams>();
        hp->ray = &mouseRay;
        cube->SetBehaviorParameters("highlight", hp);
        crate->SetBehaviorParameters("highlight", hp);
        cubeWorld->SetBehaviorParameters("highlight", hp);

        // Get the mouse ray parameters
        auto mouseRayStart = mouseRay.GetStartPoint();
        auto mouseRayEnd = mouseRay.GetPoint(50.0f);
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

        Render();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Computing Interactive Graphics");
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
        ImGui::ColorEdit3("Background color", (float*)&clearColor.r);
        ImGui::DragFloat3("Local light position", 
            (float*)&localLight.position, 0.1f);
        ImGui::Checkbox("Correct gamma", &correctGamma);
        ImGui::SliderFloat("X Angle", &cubeXAngle, 0, 360);
        ImGui::SliderFloat("Y Angle", &cubeYAngle, 0, 360);
        ImGui::SliderFloat("Z Angle", &cubeZAngle, 0, 360);
        ImGui::SliderFloat("Global Intensity", &globalLight.intensity, 0, 1);
        ImGui::SliderFloat("Local Intensity", &localLight.intensity, 0, 1);
        ImGui::SliderFloat("Sphere X Position", &spherePos.x, -5.0f, 5.0f);
        ImGui::Text("Is sphere overlapping: %s", isSphereOverlapping?"YES":"NO");
        ImGui::SliderFloat("Cuboid 1 Y Degrees", &cuboid1Deg, 0, 360);
        ImGui::SliderFloat("Cuboid 2 Y Degrees", &cuboid2Deg, 0, 360);
        ImGui::SliderFloat("Cuboid X Position", &cuboidPos.x, -5.0f, 5.0f);
        ImGui::Text("Is cuboid overlapping: %s", isCuboidOverlapping ? "YES" : "NO");
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

void GraphicsEnvironment::AddObject(const std::string& name, std::shared_ptr<GraphicsObject> object)
{
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

    //auto lookFrame = camera.GetLookFrame();
    //auto position = camera.GetPosition();
    //lookFrame[3] = glm::vec4(position, 1.0f);
    //auto invLookFrame = glm::inverse(lookFrame);

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
    const std::string& name, std::shared_ptr<GraphicsWorld> world)
{
    if (worldMap.contains(name)) return;
    worldMap[name] = world;
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
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (self->GetObject("World")->IsIntersectingWithRay(self->mouseRay)) {
            auto animation =
                std::dynamic_pointer_cast<MoveAnimation>(
                    self->GetObject("World")->GetAnimation());
            if (animation->GetState() == MoveAnimation::NOT_MOVING) {
                animation->SetState(MoveAnimation::MOVING);
            }
            else {
                animation->SetState(MoveAnimation::NOT_MOVING);
            }
            self->testStr = "World!";
        }
    }

}

void GraphicsEnvironment::OnKey(
    GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F2 && action == GLFW_PRESS) {
        self->lookWithMouse = !self->lookWithMouse;
        return;
    }
}

void GraphicsEnvironment::ProcessInput(double elapsedSeconds)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.MoveForward(elapsedSeconds);
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.MoveBackward(elapsedSeconds);
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.MoveLeft(elapsedSeconds);
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.MoveRight(elapsedSeconds);
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        camera.MoveUp(elapsedSeconds);
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera.MoveDown(elapsedSeconds);
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        glm::mat4 lookFrame(1.0f);
        camera.SetPosition({0.0f, 5.0f, 30.0f});
        camera.SetLookFrame(lookFrame);
        lookWithMouse = false;
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        glm::mat4 lookFrame(1.0f);
        lookFrame = glm::rotate(lookFrame, glm::radians(90.0f), { 0, 1, 0 });
        camera.SetPosition({30.0f, 5.0f, 0.0f });
        camera.SetLookFrame(lookFrame);
        lookWithMouse = false;
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        glm::mat4 lookFrame(1.0f);
        lookFrame = glm::rotate(lookFrame, glm::radians(180.0f), { 0, 1, 0 });
        camera.SetPosition({ 0.0f, 5.0f, -30.0f });
        camera.SetLookFrame(lookFrame);
        lookWithMouse = false;
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        glm::mat4 lookFrame(1.0f);
        lookFrame = glm::rotate(lookFrame, glm::radians(-90.0f), { 0, 1, 0 });
        camera.SetPosition({-30.0f, 5.0f, 0.0f });
        camera.SetLookFrame(lookFrame);
        lookWithMouse = false;
        return;
    }
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
