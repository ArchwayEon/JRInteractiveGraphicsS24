#include "Ray.h"

void Ray::Set(glm::vec3 start, glm::vec3 end)
{
	direction = glm::normalize(end - start);
	startPoint = start;
}
