#pragma once
class ObjectSpatialMap
{
protected:
	int numberOfRows, numberOfCols;
	float cellWidth, cellDepth;
	float totalWidth, totalDepth;

public:
	ObjectSpatialMap(
		int numberOfRows = 5, int numberOfCols = 5,
		float cellWidth = 10.0f, float cellDepth = 10.0f);

	int GetRow(float z) const {
		float nz = totalDepth / static_cast<float>(2) - z;
		return (numberOfRows - 1) - int(nz / cellDepth);
	}

	int GetCol(float x) const {
		float nx = x + totalWidth / static_cast<float>(2);
		return int(nx / cellWidth);
	}
};

