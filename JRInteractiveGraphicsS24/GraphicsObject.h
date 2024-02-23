#pragma once
#include <glm/glm.hpp>
#include <memory>

#include "VertexBuffer.h"
#include "BaseObject.h"

class IAnimation;

class GraphicsObject : public BaseObject
{
protected:
	glm::mat4 referenceFrame;
	std::shared_ptr<VertexBuffer> buffer;
	GraphicsObject* parent;
	std::vector<std::shared_ptr<GraphicsObject>> children;
	std::shared_ptr<IAnimation> animation = nullptr;

public:
	GraphicsObject();
	virtual ~GraphicsObject();

	const glm::mat4 GetReferenceFrame() const;
	glm::mat4& GetLocalReferenceFrame() { return referenceFrame; }
	inline void SetReferenceFrame(const glm::mat4& referenceFrame) {
		this->referenceFrame = referenceFrame;
	}
	void CreateVertexBuffer(unsigned int numberOfElementsPerVertex);
	void SetVertexBuffer(std::shared_ptr<VertexBuffer> buffer);
	inline const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const {
		return buffer;
	}
	void StaticAllocateVertexBuffer();

	void AddChild(std::shared_ptr<GraphicsObject> child);
	inline const std::vector<std::shared_ptr<GraphicsObject>>& GetChildren() const {
		return children;
	}

	void SetPosition(const glm::vec3& position);
	void ResetOrientation();
	void RotateLocalZ(float degrees);

	void Update(double elapsedSeconds);
	void SetAnimation(std::shared_ptr<IAnimation> animation);
};

