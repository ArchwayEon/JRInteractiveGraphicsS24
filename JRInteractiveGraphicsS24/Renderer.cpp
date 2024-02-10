#include "Renderer.h"
#include <glad/glad.h> 

Renderer::Renderer(std::shared_ptr<Shader> shader)
{
    this->shader = shader;
    glGenVertexArrays(1, &vaoId);
}

void Renderer::StaticAllocateVertexBuffers(const std::vector<std::shared_ptr<GraphicsObject>>& objects)
{
	glBindVertexArray(vaoId);
	for (auto& object : objects) {
		object->StaticAllocateVertexBuffer();
	}
	glBindVertexArray(0);
}

void Renderer::RenderScene(std::shared_ptr<Scene> scene, const glm::mat4& view)
{
	glUseProgram(shader->GetShaderProgram());
	glBindVertexArray(vaoId);
	shader->SendMat4Uniform("view", view);
	// Render the objects in the scene
	auto& objects = scene->GetObjects();
	for (auto& object : objects) {
		RenderObject(*object);
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glUseProgram(0);
	glBindVertexArray(0);
}

void Renderer::RenderObject(const GraphicsObject& object)
{
	shader->SendMat4Uniform("world", object.GetReferenceFrame());

	auto& buffer = object.GetVertexBuffer();
	buffer->Select();
	if (buffer->HasTexture()) {
		shader->SendIntUniform("tex", buffer->GetTextureUnit());
		buffer->GetTexture()->SelectToRender();
	}
	buffer->SetUpAttributeInterpretration();
	glDrawArrays(buffer->GetPrimitiveType(), 0, buffer->GetNumberOfVertices());

	// Recursively render the children
	auto& children = object.GetChildren();
	for (auto& child : children) {
		RenderObject(*child);
	}
}
