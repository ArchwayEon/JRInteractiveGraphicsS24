#pragma once
#include <glm/glm.hpp>
#include <memory>

#include "VertexBuffer.h"
#include "BaseObject.h"
#include "GraphicsStructures.h"
#include "IndexBuffer.h"
#include "BoundingBox.h"
#include "IBehavior.h"
#include "BoundingSphere.h"
#include "ReferenceFrame.h"

class IAnimation;
class IGenerator;

class GraphicsObject : public BaseObject
{
protected:
	//glm::mat4 referenceFrame;
	ReferenceFrame referenceFrame;
	std::shared_ptr<VertexBuffer> vertexBuffer = nullptr;
	std::shared_ptr<IndexBuffer> indexBuffer = nullptr;
	GraphicsObject* parent;
	std::vector<std::shared_ptr<GraphicsObject>> children;
	std::shared_ptr<IAnimation> animation = nullptr;
	std::unordered_map<std::string, std::shared_ptr<IBehavior>> behaviorMap;
	Material material{};
	std::shared_ptr<BoundingBox> boundingBox = nullptr;
	std::shared_ptr<BoundingSphere> boundingSphere = nullptr;
	std::shared_ptr<IGenerator> generator = nullptr;
	bool isOverlapping = false;

public:
	GraphicsObject();
	virtual ~GraphicsObject();

	ReferenceFrame GetReferenceFrame();
	ReferenceFrame& GetLocalReferenceFrame() { return referenceFrame; }
	inline void SetReferenceFrame(const ReferenceFrame& referenceFrame) {
		this->referenceFrame = referenceFrame;
	}
	void CreateVertexBuffer(unsigned int numberOfElementsPerVertex);
	void CreateIndexBuffer();
	void SetVertexBuffer(std::shared_ptr<VertexBuffer> buffer);
	std::shared_ptr<VertexBuffer>& GetVertexBuffer(){
		return vertexBuffer;
	}
	std::shared_ptr<IndexBuffer>& GetIndexBuffer(){
		return indexBuffer;
	}
	bool IsIndexed() const { return indexBuffer != nullptr; }
	void AllocateBuffers();

	void AddChild(std::shared_ptr<GraphicsObject> child);
	inline const std::vector<std::shared_ptr<GraphicsObject>>& GetChildren() const {
		return children;
	}

	void SetPosition(const glm::vec3& position);
	const glm::vec3 GetPosition() const { 
		return referenceFrame.GetPosition(); 
	}
	void ResetOrientation();
	void RotateLocalZ(float degrees);

	void Update(double elapsedSeconds);
	void SetAnimation(std::shared_ptr<IAnimation> animation);

	Material& GetMaterial() { return material; }
	void PointAt(const glm::vec3& target);

	void CreateBoundingBox(float width, float height, float depth);
	const BoundingBox& GetBoundingBox() const { return *boundingBox; }
	void CreateBoundingSphere(float radius);
	std::shared_ptr<BoundingSphere> GetBoundingSphere() const { 
		return boundingSphere; 
	}
	bool OverlapsWithBoundingSphere(const GraphicsObject& otherObject) const;
	bool OverlapsWithBoundingBox(const GraphicsObject& otherObject) const;
	bool IsIntersectingWithRay(const Ray& ray) const;

	void AddBehavior(std::string name, std::shared_ptr<IBehavior> behavior);
	void SetBehaviorDefaults();
	void SetBehaviorParameters(
		std::string name, std::shared_ptr<IParams> params);

	std::shared_ptr<IAnimation> GetAnimation() { return animation; }

	void SetGenerator(std::shared_ptr<IGenerator> generator) {
		this->generator = generator;
	}
	std::shared_ptr<IGenerator> GetGenerator() {
		return generator;
	}

	void Generate(DynamicBufferFlag flag = NoDynamicBuffer);

	void SetUpDynamicBuffers(
		std::size_t maxNumberOfVertices, std::size_t maxNumberOfIndices);

	void SetIsOverlapping(bool isOverlapping) { 
		this->isOverlapping = isOverlapping; 
	}
	bool IsOverlapping() const { return isOverlapping; }
};

