#include "GraphicsObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include "IAnimation.h"

GraphicsObject::GraphicsObject() : referenceFrame(1.0f), parent(nullptr)
{
	material.ambientIntensity = 0.01f;
	material.specularIntensity = 0.01f;
	material.shininess = 16.0f;
	CreateBoundingBox(1.0f, 1.0f, 1.0f);
}

GraphicsObject::~GraphicsObject()
{
}

const glm::mat4 GraphicsObject::GetReferenceFrame() const
{
	if (parent != nullptr) {
		return parent->referenceFrame * referenceFrame;
	}
	return referenceFrame;
}

void GraphicsObject::CreateVertexBuffer(unsigned int numberOfElementsPerVertex)
{
	vertexBuffer = std::make_shared<VertexBuffer>(numberOfElementsPerVertex);
}

void GraphicsObject::CreateIndexBuffer()
{
	indexBuffer = std::make_shared<IndexBuffer>();
}

void GraphicsObject::SetVertexBuffer(std::shared_ptr<VertexBuffer> buffer)
{
	this->vertexBuffer = buffer;
}

void GraphicsObject::StaticAllocateBuffers()
{
	vertexBuffer->Select();
	vertexBuffer->StaticAllocate();
	vertexBuffer->Deselect();
	if (indexBuffer != nullptr) {
		indexBuffer->Select();
		indexBuffer->StaticAllocate();
		indexBuffer->Deselect();
	}
	for (auto& child : children) {
		child->StaticAllocateBuffers();
	}
}

void GraphicsObject::AddChild(std::shared_ptr<GraphicsObject> child)
{
	children.push_back(child);
	child->parent = this;
}

void GraphicsObject::SetPosition(const glm::vec3& position)
{
	referenceFrame[3] = glm::vec4(position, 1.0f);
}

void GraphicsObject::ResetOrientation()
{
	glm::vec4 position = referenceFrame[3];
	referenceFrame = glm::mat4(1.0f);
	referenceFrame[3] = position;
}

void GraphicsObject::RotateLocalZ(float degrees)
{
	referenceFrame = glm::rotate(
		referenceFrame, 
		glm::radians(degrees), 
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
}

void GraphicsObject::Update(double elapsedSeconds)
{
	if (animation != nullptr) {
		animation->Update(elapsedSeconds);
	}
	for (auto& [name, behavior] : behaviorMap) {
		behavior->Update(elapsedSeconds);
	}
}

void GraphicsObject::SetAnimation(std::shared_ptr<IAnimation> animation)
{
	this->animation = animation;
}

void GraphicsObject::PointAt(const glm::vec3& target)
{
	glm::vec3 position = referenceFrame[3];
	glm::vec3 zAxis = glm::normalize(target - position);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 xAxis = glm::normalize(glm::cross(up, zAxis));
	glm::vec3 yAxis = glm::cross(zAxis, xAxis);
	referenceFrame[0] = glm::vec4(xAxis, 0.0f);
	referenceFrame[1] = glm::vec4(yAxis, 0.0f);
	referenceFrame[2] = glm::vec4(zAxis, 0.0f);
}

void GraphicsObject::CreateBoundingBox(float width, float height, float depth)
{
	boundingBox = std::make_shared<BoundingBox>();
	boundingBox->SetReferenceFrame(referenceFrame);
	boundingBox->Create(width, height, depth);
}

bool GraphicsObject::IsIntersectingWithRay(const Ray& ray) const
{
	boundingBox->SetReferenceFrame(referenceFrame);
	return boundingBox->IsIntersectingWithRay(ray);
}

void GraphicsObject::AddBehavior(
	std::string name, std::shared_ptr<IBehavior> behavior)
{
	if (behaviorMap.contains(name)) return;
	behaviorMap[name] = behavior;
}

void GraphicsObject::SetBehaviorDefaults()
{
	for (auto& [name, behavior] : behaviorMap) {
		behavior->StoreDefaults();
	}
}

void GraphicsObject::SetBehaviorParameters(std::string name, IParams params)
{
	if (!behaviorMap.contains(name)) return;
	behaviorMap[name]->SetParameter(params);
}
