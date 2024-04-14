#include "ReferenceFrame.h"
#include <glm/gtc/matrix_transform.hpp>

void ReferenceFrame::PointAt(const glm::vec3& target)
{
	glm::vec3 position = matrix4[3];
	glm::vec3 zAxis = glm::normalize(target - position);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 xAxis = glm::normalize(glm::cross(up, zAxis));
	glm::vec3 yAxis = glm::cross(zAxis, xAxis);
	matrix4[0] = glm::vec4(xAxis, 0.0f);
	matrix4[1] = glm::vec4(yAxis, 0.0f);
	matrix4[2] = glm::vec4(zAxis, 0.0f);
}

void ReferenceFrame::SetPosition(glm::vec3 position)
{
	matrix4[3].x = position.x;
	matrix4[3].y = position.y;
	matrix4[3].z = position.z;
	matrix4[3].w = 1.0f;
}

void ReferenceFrame::RotateX(float deltaDegrees)
{
	matrix4 = glm::rotate(
		matrix4, glm::radians(deltaDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
}

void ReferenceFrame::RotateY(float deltaDegrees)
{
	matrix4 = glm::rotate(
		matrix4, glm::radians(deltaDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
}

void ReferenceFrame::RotateZ(float deltaDegrees)
{
	matrix4 = glm::rotate(
		matrix4, glm::radians(deltaDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
}

void ReferenceFrame::RotateAxis(glm::vec3 axis, float deltaDegrees)
{
	matrix4 = glm::rotate(
		matrix4, glm::radians(deltaDegrees), axis);
}

ReferenceFrame ReferenceFrame::CombineWithParentFrame(
	const ReferenceFrame& parent)
{
	return ReferenceFrame(parent.matrix4 * matrix4);
}

glm::vec3 ReferenceFrame::WorldToLocal(glm::vec4 wv) const
{
	glm::mat4 invMat = glm::inverse(matrix4);
	return invMat * wv;
}

glm::vec3 ReferenceFrame::WorldToLocal(glm::vec3 wv) const
{
	return WorldToLocal(glm::vec4(wv, 1.0f));
}

glm::vec3 ReferenceFrame::LocalToWorld(glm::vec4 lv) const
{
	return matrix4 * lv;
}
