#pragma once
#include "BaseObject.h"
#include "Shader.h"
#include "GraphicsObject.h"
#include "Scene.h"
#include <memory>
#include <vector>

class Renderer : public BaseObject
{
private:
    unsigned int vaoId;
    std::shared_ptr<Shader> shader;

public:
    Renderer(std::shared_ptr<Shader> shader);
    void AllocateVertexBuffers(const std::vector<std::shared_ptr<GraphicsObject>>& objects);
    inline const std::shared_ptr<Shader>& GetShader() const { return shader; }
    void RenderScene(std::shared_ptr<Scene> scene, const glm::mat4& view);

private:
    void RenderObject(const GraphicsObject& object);
};

