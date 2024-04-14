#pragma once
#include <memory>

class GraphicsEnvironment;

class GraphicsWorld
{
protected:
	std::shared_ptr<GraphicsEnvironment> _env;

public:
	GraphicsWorld(std::shared_ptr<GraphicsEnvironment> env);
	void Create();
private:
	void CreateRenderers();
	// Specializations for each world.
	void CreateScene1();
	void CreateScene2();
	void CreateScene3();

	void CreateRenderer1();
	void CreateRenderer2();
	void CreateRenderer3();
};

