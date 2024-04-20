#pragma once
#include "IGraphicsWorld.h"
#include "GraphicsStructures.h"
#include "GeometricPlane.h"
#include <memory>

class GraphicsEnvironment;
class BVHNode;

class BVHWorld :  public IGraphicsWorld
{
protected:
	Light localLight{}, globalLight{};
	int numberOfCrates = 20;
	glm::vec3 mouseRayStart = { 0.0f, 0.0f, 0.0f };
	glm::vec3 mouseRayEnd = { 0.0f, 0.0f, 0.0f };
	GeometricPlane floorPlane;
	glm::vec3 floorIntersectionPoint{};
	std::shared_ptr<BVHNode> rootBVH;
	int numberOfCollisions = 0;
	int numPotentialCollisions = 0;

public:
	BVHWorld(std::shared_ptr<GraphicsEnvironment> env);
	void Create() override;
	void Preupdate() override;;
	void Update(float elapsedSeconds) override;;
	void UI(ImGuiIO& io) override;
	void OnMouseButton(int button, int action, int mods) override;
	void OnKey(int key, int scancode, int action, int mods) override;
	void PollInputs(float elapsedSeconds) override;
private:
	void CreateRenderers() override;
	void CreateScene1();
	void CreateScene2();
	void CreateScene3();
	void CreateRenderer1();
	void CreateRenderer2();
	void CreateRenderer3();
	void ResetIsOverlapping();
	void InsertObjectsIntoBVH();
	void CheckForCollisions();
};

