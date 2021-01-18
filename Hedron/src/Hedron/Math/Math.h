#pragma once

#include <glm/glm.hpp>

namespace Hedron::Math
{
	bool decompose_transform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);
}