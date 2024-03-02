#include "Renderer.h"
#include <glad/glad.h> 

Renderer::Renderer(std::shared_ptr<Shader> shader)
{
	this->view = glm::mat4(1.0f);
	this->projection = glm::mat4(1.0f);
    this->shader = shader;
    glGenVertexArrays(1, &vaoId);
}

void Renderer::StaticAllocateVertexBuffers()
{
	glBindVertexArray(vaoId);
	auto& objects = scene->GetObjects();
	for (auto& object : objects) {
		object->StaticAllocateVertexBuffer();
	}
	glBindVertexArray(0);
}

void Renderer::RenderScene(const Camera& camera)
{
	glUseProgram(shader->GetShaderProgram());
	glBindVertexArray(vaoId);
	shader->SendMat4Uniform("projection", projection);
	shader->SendMat4Uniform("view", view);
	Light& gl = scene->GetGlobalLight();
	shader->SendVec3Uniform("globalLightPosition", gl.position);
	shader->SendVec3Uniform("globalLightColor", gl.color);
	shader->SendFloatUniform("globalLightIntensity", gl.intensity);
	Light& ll = scene->GetLocalLight();
	shader->SendVec3Uniform("localLightPosition", ll.position);
	shader->SendVec3Uniform("localLightColor", ll.color);
	shader->SendFloatUniform("localLightIntensity", ll.intensity);
	shader->SendFloatUniform("localLightAttenuationCoef", ll.attenuationCoef);
	shader->SendVec3Uniform("viewPosition", camera.GetPosition());
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

void Renderer::RenderObject(GraphicsObject& object)
{
	shader->SendMat4Uniform("world", object.GetReferenceFrame());
	Material& m = object.GetMaterial();
	shader->SendFloatUniform("materialAmbientIntensity", m.ambientIntensity);
	shader->SendFloatUniform("materialSpecularIntensity", m.specularIntensity);
	shader->SendFloatUniform("materialShininess", m.shininess);
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
