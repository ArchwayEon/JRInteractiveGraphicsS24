#include "VertexBuffer.h"
#include <cstdarg>


VertexBuffer::VertexBuffer(unsigned int numElementsPerVertex)
{
	numberOfElementsPerVertex = numElementsPerVertex;
	//numberOfVertices = 0;
	primitiveType = GL_TRIANGLES;
	glGenBuffers(1, &vboId);
	textureUnit = 0;
	texture = nullptr;
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &vboId);
}

void VertexBuffer::AddVertexData(unsigned int count, ...)
{
	if (count != numberOfElementsPerVertex) {
		throw "Invalid vertex data count!";
	}
	va_list args;
	va_start(args, count);
	while (count > 0) {
		// The default is double, so accept as double and then cast to
		// float.
		vertexData.push_back(static_cast<float>(va_arg(args, double)));
		count--;
	}
	//numberOfVertices++;
	va_end(args);
}

void VertexBuffer::Allocate()
{
	unsigned long long bytesToAllocate;
	if (isDynamic == false) {
		bytesToAllocate = vertexData.size() * sizeof(float);
		glBufferData(
			GL_ARRAY_BUFFER, bytesToAllocate, vertexData.data(), GL_STATIC_DRAW);
	}
	else {
		unsigned long long numberOfElements =
			unsigned long long(maxNumberOfVertices) * numberOfElementsPerVertex;
		bytesToAllocate = numberOfElements * sizeof(float);
		glBufferData(
			GL_ARRAY_BUFFER, bytesToAllocate, nullptr, GL_DYNAMIC_DRAW);
	}
	
	if (texture != nullptr) {
		texture->Allocate();
	}
}

void VertexBuffer::AddVertexAttribute(
	const std::string& name, unsigned int index, 
	unsigned int numberOfElements, unsigned int offsetCount)
{
	unsigned long long vertexSizeInBytes = sizeof(float) * numberOfElementsPerVertex;
	unsigned long long bytesToNext = vertexSizeInBytes;
	unsigned long long offsetBytes = sizeof(float) * offsetCount;
	VertexAttribute attr = { 
		index, numberOfElements, GL_FLOAT, GL_FALSE, 
		(unsigned int)bytesToNext, (void*)offsetBytes 
	};
	attributeMap[name] = attr;
}

void VertexBuffer::SetUpAttributeInterpretration()
{
	for (const std::pair<const std::string, VertexAttribute>& item : attributeMap) {
		const auto& attr = item.second;
		glEnableVertexAttribArray(attr.index);
		glVertexAttribPointer(
			attr.index, attr.numberOfComponents, attr.type,
			attr.isNormalized, attr.bytesToNext, attr.byteOffset
		);
	}
}

void VertexBuffer::SelectTexture() const
{
	if (texture != nullptr) {
		texture->SelectToRender(textureUnit);
	}
}

void VertexBuffer::SendData()
{
	unsigned long long sizeInBytes = vertexData.size() * sizeof(float);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeInBytes, vertexData.data());
}
