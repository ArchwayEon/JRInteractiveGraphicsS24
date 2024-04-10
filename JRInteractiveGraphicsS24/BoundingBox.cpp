#include "BoundingBox.h"
#include <algorithm>

void BoundingBox::Create(float width, float height, float depth)
{
	this->width = width;
	this->height = height;
	this->depth = depth;
	float hw = this->width / 2;
	float hh = this->height / 2;
	float hd = this->depth / 2;
	glm::vec3 xAxis = frame[0];
	glm::vec3 yAxis = frame[1];
	glm::vec3 zAxis = frame[2];
	planes[FRONT].Set(-zAxis, hd);
	planes[BACK].Set(zAxis, hd);
	planes[RIGHT].Set(-xAxis, hw);
	planes[LEFT].Set(xAxis, hw);
	planes[TOP].Set(-yAxis, hh);
	planes[BOTTOM].Set(yAxis, hh);
}

bool BoundingBox::IsIntersectingWithRay(const Ray& ray)
{
	intersections.clear();
	float intersection;
	Ray localRay;
	glm::vec3 localStart = glm::vec3(
		invFrame * glm::vec4(ray.GetStartPoint(), 1.0f));
	localRay.SetStartPoint(localStart);
	glm::vec3 localDir = glm::vec3(
		invFrame * glm::vec4(ray.GetDirection(), 0.0f));
	localRay.SetDirection(localDir);
	for (int i = FRONT; i <= BOTTOM; i++) {
		intersection = planes[i].GetIntersectionOffset(localRay);
		intersections.push_back(intersection);
	}

	// Test intersection with the 2 planes perpendicular to the OBB's X axis
	float nearestFarI = intersections[BoundingBox::BACK];
	float farthestNearI = intersections[BoundingBox::FRONT];
	if (nearestFarI < farthestNearI) {
		std::swap(nearestFarI, farthestNearI);
	}
	float nearI = intersections[BoundingBox::LEFT];
	float farI = intersections[BoundingBox::RIGHT];
	if (nearI > farI) {
		std::swap(nearI, farI);
	}
	if (nearI != farI) {
		if (farI < nearestFarI) nearestFarI = farI;
		if (nearI > farthestNearI) farthestNearI = nearI;
		if (nearestFarI < farthestNearI) return false;
	}

	// Test intersection with the 2 planes perpendicular to the OBB's Y axis
	nearI = intersections[BoundingBox::FRONT];
	farI = intersections[BoundingBox::BACK];
	if (nearI > farI) {
		std::swap(nearI, farI);
	}
	if (nearI != farI) {
		if (farI < nearestFarI) nearestFarI = farI;
		if (nearI > farthestNearI) farthestNearI = nearI;
		if (nearestFarI < farthestNearI) return false;
	}

	// Test intersection with the 2 planes perpendicular to the OBB's Z axis
	nearI = intersections[BoundingBox::TOP];
	farI = intersections[BoundingBox::BOTTOM];
	if (nearI > farI) {
		std::swap(nearI, farI);
	}
	if (nearI != farI) {
		if (farI < nearestFarI) nearestFarI = farI;
		if (nearI > farthestNearI) farthestNearI = nearI;
		if (nearestFarI < farthestNearI) return false;
	}

	intersectionPoint = ray.GetPoint(farthestNearI);
	return true;
}

MinMax BoundingBox::GetMinMaxProjection(const glm::vec3& axis) const
{
	float halfWidth = width / 2;
	float halfHeight = height / 2;
	float halfDepth = depth / 2;
	glm::vec3 v[8]{};
	// Front Face
	v[0] = frame * glm::vec4(-halfWidth, halfHeight, halfDepth, 1.0f);
	v[1] = frame * glm::vec4(-halfWidth, -halfHeight, halfDepth, 1.0f);
	v[2] = frame * glm::vec4(halfWidth, -halfHeight, halfDepth, 1.0f);
	v[3] = frame * glm::vec4(halfWidth, halfHeight, halfDepth, 1.0f);
	// Back Face
	v[4] = frame * glm::vec4(-halfWidth, halfHeight, -halfDepth, 1.0f);
	v[5] = frame * glm::vec4(-halfWidth, -halfHeight, -halfDepth, 1.0f);
	v[6] = frame * glm::vec4(halfWidth, -halfHeight, -halfDepth, 1.0f);
	v[7] = frame * glm::vec4(halfWidth, halfHeight, -halfDepth, 1.0f);

	MinMax m;
	m.min = m.max = glm::dot(v[0], axis);
	float val;
	for (auto i = 1; i < 8; i++) {
		val = glm::dot(v[i], axis);
		if (val < m.min) {
			m.min = val;
		}
		if (val > m.max) {
			m.max = val;
		}
	}
	return m;
}

bool BoundingBox::OverlapsWith(const BoundingBox& other) const
{
	// 1
	glm::vec3 xAxis = frame[0];
	MinMax b1 = GetMinMaxProjection(xAxis);
	MinMax b2 = other.GetMinMaxProjection(xAxis);
	if (b1.max < b2.min || b2.max < b1.min) return false;
	// 2
	glm::vec3 xAxisOther = other.frame[0];
	b1 = GetMinMaxProjection(xAxisOther);
	b2 = other.GetMinMaxProjection(xAxisOther);
	if (b1.max < b2.min || b2.max < b1.min) return false;
	// 3
	glm::vec3 yAxis = frame[1];
	b1 = GetMinMaxProjection(yAxis);
	b2 = other.GetMinMaxProjection(yAxis);
	if (b1.max < b2.min || b2.max < b1.min) return false;
	// 4
	glm::vec3 yAxisOther = other.frame[1];
	b1 = GetMinMaxProjection(yAxisOther);
	b2 = other.GetMinMaxProjection(yAxisOther);
	if (b1.max < b2.min || b2.max < b1.min) return false;
	// 5
	glm::vec3 zAxis = frame[2];
	b1 = GetMinMaxProjection(zAxis);
	b2 = other.GetMinMaxProjection(zAxis);
	if (b1.max < b2.min || b2.max < b1.min) return false;
	// 6
	glm::vec3 zAxisOther = other.frame[2];
	b1 = GetMinMaxProjection(zAxisOther);
	b2 = other.GetMinMaxProjection(zAxisOther);
	if (b1.max < b2.min || b2.max < b1.min) return false;
	// 7
	glm::vec3 xxedge = glm::normalize(glm::cross(xAxis, xAxisOther));
	b1 = GetMinMaxProjection(xxedge);
	b2 = other.GetMinMaxProjection(xxedge);
	if (b1.max < b2.min || b2.max < b1.min) return false;
	// 8
	glm::vec3 xyedge = glm::normalize(glm::cross(xAxis, yAxisOther));
	b1 = GetMinMaxProjection(xyedge);
	b2 = other.GetMinMaxProjection(xyedge);
	if (b1.max < b2.min || b2.max < b1.min) return false;
	// 9
	glm::vec3 xzedge = glm::normalize(glm::cross(xAxis, zAxisOther));
	b1 = GetMinMaxProjection(xzedge);
	b2 = other.GetMinMaxProjection(xzedge);
	if (b1.max < b2.min || b2.max < b1.min) return false;
	// 10
	glm::vec3 yxedge = glm::normalize(glm::cross(yAxis, xAxisOther));
	b1 = GetMinMaxProjection(yxedge);
	b2 = other.GetMinMaxProjection(yxedge);
	if (b1.max < b2.min || b2.max < b1.min) return false;
	// 11
	glm::vec3 yyedge = glm::normalize(glm::cross(yAxis, yAxisOther));
	b1 = GetMinMaxProjection(yyedge);
	b2 = other.GetMinMaxProjection(yyedge);
	if (b1.max < b2.min || b2.max < b1.min) return false;
	// 12
	glm::vec3 yzedge = glm::normalize(glm::cross(yAxis, zAxisOther));
	b1 = GetMinMaxProjection(yzedge);
	b2 = other.GetMinMaxProjection(yzedge);
	if (b1.max < b2.min || b2.max < b1.min) return false;
	// 13
	glm::vec3 zxedge = glm::normalize(glm::cross(zAxis, xAxisOther));
	b1 = GetMinMaxProjection(zxedge);
	b2 = other.GetMinMaxProjection(zxedge);
	if (b1.max < b2.min || b2.max < b1.min) return false;
	// 14
	glm::vec3 zyedge = glm::normalize(glm::cross(zAxis, yAxisOther));
	b1 = GetMinMaxProjection(zyedge);
	b2 = other.GetMinMaxProjection(zyedge);
	if (b1.max < b2.min || b2.max < b1.min) return false;
	// 15
	glm::vec3 zzedge = glm::normalize(glm::cross(zAxis, zAxisOther));
	b1 = GetMinMaxProjection(zzedge);
	b2 = other.GetMinMaxProjection(zzedge);
	if (b1.max < b2.min || b2.max < b1.min) return false;

	return true;
}
