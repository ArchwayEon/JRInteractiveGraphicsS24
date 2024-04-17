#pragma once
#include <vector>
#include <memory>

class GraphicsObject;

class ObjectSpatialMap
{
protected:
	int numberOfRows, numberOfCols;
	float cellWidth, cellDepth;
	float totalWidth, totalDepth;
	std::vector<std::vector<std::shared_ptr<GraphicsObject>>> objects;
	std::vector<std::shared_ptr<GraphicsObject>> empty;

public:
	ObjectSpatialMap(
		int numberOfRows = 5, int numberOfCols = 5,
		float cellWidth = 10.0f, float cellDepth = 10.0f);

	void Clear();

	int GetRow(float z) const {
		float nz = totalDepth / 2.0f - z;
		return (numberOfRows - 1) - int(nz / cellDepth);
	}

	int GetCol(float x) const {
		float nx = x + totalWidth / 2.0f;
		return int(nx / cellWidth);
	}

	std::vector<std::shared_ptr<GraphicsObject>> GetObjects(std::size_t row, std::size_t col);
	void AddObject(std::shared_ptr<GraphicsObject> object);
};

