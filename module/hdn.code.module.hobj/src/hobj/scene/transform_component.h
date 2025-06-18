#pragma once

#include "core/core.h"

namespace hdn
{
	struct TransformComponent
	{
		vec3f32 position{};
		vec3f32 scale{ 1.0f, 1.0f, 1.0f };
		vec3f32 rotation{}; // In radians (Euler YXZ)

		mat4f32 worldMatrix; // Populated at runtime

		mat4f32 to_mat();
		mat3f32 normal_matrix();
	};
}
