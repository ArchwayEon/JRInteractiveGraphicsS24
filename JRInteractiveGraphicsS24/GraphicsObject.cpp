#include "GraphicsObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include "IAnimation.h"
#include "IGenerator.h"

GraphicsObject::GraphicsObject() : referenceFrame(), parent(nullptr)
{
	material.ambientIntensity = 0.01f;
	material.specularIntensity = 0.01f;
	material.shininess = 16.0f;
	CreateBoundingBox(1.0f, 1.0f, 1.0f);
}

GraphicsObject::~GraphicsObject()
{
}

ReferenceFrame GraphicsObject::GetReferenceFrame()
{
	if (parent != nullptr) {
		return referenceFrame.CombineWithParentFrame(parent->referenceFrame);
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

void GraphicsObject::AllocateBuffers()
{
	vertexBuffer->Select();
	vertexBuffer->Allocate();
	vertexBuffer->Deselect();
	if (indexBuffer != nullptr) {
		indexBuffer->Select();
		indexBuffer->Allocate();
		indexBuffer->Deselect();
	}
	for (auto& child : children) {
		child->AllocateBuffers();
	}
}

void GraphicsObject::AddChild(std::shared_ptr<GraphicsObject> child)
{
	children.push_back(child);
	child->parent = this;
}

void GraphicsObject::SetPosition(const glm::vec3& position)
{
	referenceFrame.SetPosition(position);
}

void GraphicsObject::ResetOrientation()
{
	referenceFrame.Reset();
}

void GraphicsObject::RotateLocalZ(float degrees)
{
	referenceFrame.RotateZ(degrees);
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
	glm::vec3 position = referenceFrame.GetPosition();
	glm::vec3 zAxis = glm::normalize(target - position);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 xAxis = glm::normalize(glm::cross(up, zAxis));
	glm::vec3 yAxis = glm::cross(zAxis, xAxis);
	referenceFrame.SetXAxis(xAxis);
	referenceFrame.SetYAxis(yAxis);
	referenceFrame.SetZAxis(zAxis);
}

void GraphicsObject::CreateBoundingBox(float width, float height, float depth)
{
	boundingBox = std::make_shared<BoundingBox>();
	boundingBox->SetReferenceFrame(referenceFrame);
	boundingBox->Create(width, height, depth);
}

void GraphicsObject::CreateBoundingSphere(float radius)
{
	boundingSphere= std::make_shared<BoundingSphere>(radius);
	boundingSphere->SetPosition(referenceFrame.GetPosition());
}

bool GraphicsObject::OverlapsWithBoundingSphere(const GraphicsObject& otherObject) const
{
	if (boundingSphere != nullptr) {
		if (otherObject.boundingSphere != nullptr) {
			boundingSphere->SetPosition(GetPosition());
			otherObject.boundingSphere->SetPosition(otherObject.GetPosition());
			return boundingSphere->OverlapsWith(*otherObject.boundingSphere);
		}
	}
	return false;
}

bool GraphicsObject::OverlapsWithBoundingBox(const GraphicsObject& otherObject) const
{
	if (boundingBox != nullptr) {
		if (otherObject.boundingBox != nullptr) {
			boundingBox->SetReferenceFrame(referenceFrame);
			otherObject.boundingBox->SetReferenceFrame(
				otherObject.referenceFrame);
			return boundingBox->OverlapsWith(*otherObject.boundingBox);
		}
	}
	return false;
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

void GraphicsObject::SetBehaviorParameters(
	std::string name, std::shared_ptr<IParams> params)
{
	if (!behaviorMap.contains(name)) return;
	behaviorMap[name]->SetParameter(params);
}

void GraphicsObject::Generate(DynamicBufferFlag flag)
{
	if (generator != nullptr) {
		generator->Generate();
		if (flag == UseDynamicBuffers) {
			generator->SetUpDynamicBuffers();
		}
	}
}

void GraphicsObject::SetUpDynamicBuffers(
	std::size_t maxNumberOfVertices, std::size_t maxNumberOfIndices)
{
	vertexBuffer->SetIsDynamic(true);
	vertexBuffer->SetMaxNumberOfVertices(maxNumberOfVertices);
	indexBuffer->SetIsDynamic(true);
	indexBuffer->SetMaxNumberOfIndices(maxNumberOfIndices);
}
