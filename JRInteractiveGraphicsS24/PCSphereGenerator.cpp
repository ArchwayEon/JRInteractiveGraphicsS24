#include "PCSphereGenerator.h"
#include "GraphicsObject.h"
#include "Generate.h"

PCSphereGenerator::PCSphereGenerator(
	std::shared_ptr<GraphicsObject> graphicsObject)
{
	this->graphicsObject = graphicsObject;
	this->graphicsObject->CreateVertexBuffer(6);
	this->graphicsObject->CreateIndexBuffer();
}

inline void PCSphereGenerator::SetParameters(std::shared_ptr<IParams> params)
{
	this->params = std::static_pointer_cast<PCSphereParams>(params);
}

void PCSphereGenerator::Generate()
{
	auto color = graphicsObject->GetMaterial().color;
	auto& vertexBuffer = graphicsObject->GetVertexBuffer();
	vertexBuffer->Clear();
	Generate::LineSphere(vertexBuffer, 
		params->radius, params->slices, params->stacks, color);
	vertexBuffer->SetPrimitiveType(GL_LINES);
	vertexBuffer->AddVertexAttribute("position", 0, 3, 0);
	vertexBuffer->AddVertexAttribute("color", 1, 3, 3);
	auto& indexBuffer = graphicsObject->GetIndexBuffer();
	indexBuffer->Clear();
	Generate::LineSphereIndexes(indexBuffer, params->slices, params->stacks,
		vertexBuffer->GetNumberOfVertices());
}

void PCSphereGenerator::SetUpDynamicBuffers()
{
	auto& vertexBuffer = graphicsObject->GetVertexBuffer();
	auto& indexBuffer = graphicsObject->GetIndexBuffer();
	graphicsObject->SetUpDynamicBuffers(
		vertexBuffer->GetNumberOfVertices(),
		indexBuffer->GetNumberOfIndices());
}
