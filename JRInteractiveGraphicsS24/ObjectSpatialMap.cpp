#include "ObjectSpatialMap.h"
#include "GraphicsObject.h"

ObjectSpatialMap::ObjectSpatialMap(
	int numberOfRows, int numberOfCols, 
	float cellWidth, float cellDepth) :
	numberOfRows(numberOfRows), numberOfCols(numberOfCols),
	cellWidth(cellWidth), cellDepth(cellDepth), objects()
{
	totalWidth = numberOfCols * cellWidth;
	totalDepth = numberOfRows * cellDepth;
	for (int row = 0; row < numberOfRows; row++) {
		for (int col = 0; col < numberOfCols; col++) {
			objects.push_back(std::vector<std::shared_ptr<GraphicsObject>>());
		}
	}
}

void ObjectSpatialMap::Clear()
{
	for (int row = 0; row < numberOfRows; row++) {
		for (int col = 0; col < numberOfCols; col++) {
			auto index = row * numberOfCols + col;
			objects[index].clear();
		}
	}
}

std::vector<std::shared_ptr<GraphicsObject>> ObjectSpatialMap::GetObjects(std::size_t row, std::size_t col)
{
	if (row < 0 || row > numberOfRows) return empty;
	if (col < 0 || col > numberOfCols) return empty;
	auto index = row * numberOfCols + col;
	return objects[index];
}

void ObjectSpatialMap::AddObject(std::shared_ptr<GraphicsObject> object)
{
	auto pos = object->GetPosition();
	auto row = GetRow(pos.z);
	auto col = GetCol(pos.x);
	auto index = row * numberOfCols + col;
	objects[index].push_back(object);
}
