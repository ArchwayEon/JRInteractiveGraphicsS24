#include "PCCuboidGenerator.h"
#include "GraphicsObject.h"
#include "Generate.h"


PCCuboidGenerator::PCCuboidGenerator(
	std::shared_ptr<GraphicsObject> graphicsObject)
{
	this->graphicsObject = graphicsObject;
	this->graphicsObject->CreateVertexBuffer(6);
	this->graphicsObject->CreateIndexBuffer();
}

inline void PCCuboidGenerator::SetParameters(
	std::shared_ptr<IParams> params)
{
	this->params = std::static_pointer_cast<PCCuboidParams>(params);
}

void PCCuboidGenerator::Generate()
{
	auto color = graphicsObject->GetMaterial().color;
	auto& vertexBuffer = graphicsObject->GetVertexBuffer();
	vertexBuffer->Clear();
	Generate::LineCuboid(vertexBuffer,
		params->width, params->height, params->depth, color);
	vertexBuffer->SetPrimitiveType(GL_LINES);
	vertexBuffer->AddVertexAttribute("position", 0, 3, 0);
	vertexBuffer->AddVertexAttribute("color", 1, 3, 3);
	auto& indexBuffer = graphicsObject->GetIndexBuffer();
	indexBuffer->Clear();
	Generate::LineCuboidIndices(indexBuffer);

}

void PCCuboidGenerator::SetUpDynamicBuffers()
{
	auto& vertexBuffer = graphicsObject->GetVertexBuffer();
	auto& indexBuffer = graphicsObject->GetIndexBuffer();
	graphicsObject->SetUpDynamicBuffers(
		vertexBuffer->GetNumberOfVertices(),
		indexBuffer->GetNumberOfIndices());

}


