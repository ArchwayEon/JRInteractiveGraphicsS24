#include "ObjectSpatialMap.h"

ObjectSpatialMap::ObjectSpatialMap(
	int numberOfRows, int numberOfCols, 
	float cellWidth, float cellDepth) :
	numberOfRows(numberOfRows), numberOfCols(numberOfCols),
	cellWidth(cellWidth), cellDepth(cellDepth)
{
	totalWidth = numberOfCols * cellWidth;
	totalDepth = numberOfRows * cellDepth;
}
