#pragma once
#include "IGraphicsWorld.h"
#include "GraphicsStructures.h"
#include <glm/glm.hpp>

class GraphicsEnvironment;

class ObjectMapWorld : public IGraphicsWorld
{
private:
	Light localLight{}, globalLight{};
public:
	ObjectMapWorld(std::shared_ptr<GraphicsEnvironment> env);
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
	void CreateRenderer1();
};

