#pragma once
#include "Shader.h"
#include "GraphicsObject.h"
#include "Scene.h"
#include <memory>
#include <vector>
#include "Camera.h"

class Renderer 
{
private:
    unsigned int vaoId;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Scene> scene;
    glm::mat4 view;
    glm::mat4 projection;

public:
    Renderer(std::shared_ptr<Shader> shader);
    void AllocateBuffers();
    inline const std::shared_ptr<Shader>& GetShader() const { return shader; }
    inline void SetScene(std::shared_ptr<Scene> scene) { this->scene = scene; }
    std::shared_ptr<Scene> GetScene() { return scene;  }
    inline void SetView(const glm::mat4& view) { this->view = view; }
    inline void SetProjection(const glm::mat4& projection) { this->projection = projection; }
    inline const std::shared_ptr<Scene>& GetScene() const { return scene; }
    void RenderScene(std::shared_ptr<Camera> camera);

private:
    void RenderObject(GraphicsObject& object);
};

