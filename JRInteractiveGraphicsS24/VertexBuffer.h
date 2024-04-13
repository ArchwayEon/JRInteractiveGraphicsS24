#pragma once
#include <glad/glad.h> 
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include "Texture.h"

struct VertexAttribute {
	unsigned int index;
	unsigned int numberOfComponents;
	int type;
	int isNormalized;
	unsigned int bytesToNext;
	void* byteOffset;
};

class VertexBuffer
{
protected:
	std::size_t numberOfElementsPerVertex;
	std::size_t maxNumberOfVertices = 0;
	unsigned int vboId;
	int primitiveType;
	std::vector<float> vertexData;
	std::unordered_map<std::string, VertexAttribute> attributeMap;
	int textureUnit;
	std::shared_ptr<Texture> texture;
	bool isDynamic = false;

public:
	VertexBuffer(unsigned int numElementsPerVertex = 3);
	~VertexBuffer();

	inline void Select() { glBindBuffer(GL_ARRAY_BUFFER, vboId); }
	inline void Deselect() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
	inline std::size_t GetNumberOfVertices() const { 
		return vertexData.size() / numberOfElementsPerVertex;
	}
	inline int GetPrimitiveType() const { return primitiveType; }
	inline void SetPrimitiveType(int primitiveType) { this->primitiveType = primitiveType; }

	// Variadic function
	void AddVertexData(unsigned int count, ...);
	void Allocate();
	void AddVertexAttribute(
		const std::string& name, unsigned int index, 
		unsigned int numberOfElements, unsigned int offsetCount=0);
	void SetUpAttributeInterpretration();

	void SetTextureUnit(int textureUnit) { this->textureUnit = textureUnit; }
	int GetTextureUnit() const { return textureUnit; }
	inline void SetTexture(std::shared_ptr<Texture> texture) {
		this->texture = texture;
	}
	std::shared_ptr<Texture> GetTexture() const { return texture; }
	bool HasTexture() const { return texture != nullptr; }
	void SelectTexture() const;

	void SetIsDynamic(bool isDynamic) { this->isDynamic = isDynamic; }
	bool IsDynamic() const { return isDynamic; }
	void SetMaxNumberOfVertices(std::size_t maxNumberOfVertices) {
		this->maxNumberOfVertices = maxNumberOfVertices;
	}
	std::size_t GetMaxNumberOfVertices() const {
		return maxNumberOfVertices;
	}

	void SendData();
	void Clear() { 
		vertexData.clear(); 
	}
};

