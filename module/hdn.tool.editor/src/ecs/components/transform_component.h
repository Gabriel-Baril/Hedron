#pragma once

#include "core/core.h"

namespace hdn
{
	struct TransformComponent
	{
		vec3f32 translation{};
		vec3f32 scale{ 1.0f, 1.0f, 1.0f };
		vec3f32 rotation{}; // In radians (Euler YXZ)

		mat4f32 worldMatrix;

		mat4f32 Mat4();
		mat3f32 NormalMatrix();
	};
}
