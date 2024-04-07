#pragma once
#include <memory>
#include "VertexBuffer.h"
#include <glm/glm.hpp>
#include "IndexBuffer.h"

class Generate
{
public:
	static std::shared_ptr<VertexBuffer> Cuboid(
		float width, float height, float depth,
		glm::vec3 color = { 1.0f, 1.0f, 1.0f },
		glm::vec2 tex = { 1.0f, 1.0 });
	static std::shared_ptr<VertexBuffer> CuboidWithNormals(
		float width, float height, float depth,
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f },
		glm::vec2 tex = { 1.0f, 1.0 });
	static std::shared_ptr<VertexBuffer> XZPlane(
		float width, float depth,
		glm::vec3 color = { 1.0f, 1.0f, 1.0f },
		glm::vec2 tex = { 1.0f, 1.0 });
	static std::shared_ptr<VertexBuffer> XYPlane(
		float width, float height,
		glm::vec3 color = { 1.0f, 1.0f, 1.0f },
		glm::vec2 tex = { 1.0f, 1.0 });
	static std::shared_ptr<VertexBuffer> XZPlaneWithNormals(
		float width, float depth,
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f },
		glm::vec2 tex = { 1.0f, 1.0 });

	static void XZLineCircle(
		std::shared_ptr<VertexBuffer>& bufferToFill, 
		float radius, glm::vec3 color = { 1.0f, 1.0f, 1.0f }, 
		float offset = 0.0f,
		int steps = 10);
	// Returns the highest index
	static int LineCircleIndexes(
		std::shared_ptr<IndexBuffer>& bufferToFill, 
		int numberOfLineSegments, bool isClosed = true, int startIndex = 0);

	static void LineCylinder(
		std::shared_ptr<VertexBuffer>& bufferToFill,
		float height, float radius, glm::vec3 color = { 1.0f, 1.0f, 1.0f }, 
		int steps = 10);

	
	static void LineCylinderIndexes(
		std::shared_ptr<IndexBuffer>& bufferToFill,	
		int numberOfCircleLineSegments);

	static void LineSphere(
		std::shared_ptr<VertexBuffer>& bufferToFill,
		float radius, int slices, int stacks, 
		glm::vec3 color = { 1.0f, 1.0f, 1.0f });

	static void LineSphereIndexes(
		std::shared_ptr<IndexBuffer>& bufferToFill, int slices, int stacks, 
		std::size_t numberOfVertices);
};

