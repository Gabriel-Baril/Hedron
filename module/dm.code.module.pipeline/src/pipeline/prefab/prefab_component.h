#pragma once

#include "core/core.h"

namespace hdn
{
	struct PrefabHierarchyComponent
	{
		int dummy;
	};

	struct PrefabModelComponent
	{
		int dummy;
	};

	struct PrefabTransformComponent
	{
		vec3f32 position{};
		vec3f32 scale{ 1.0f, 1.0f, 1.0f };
		vec3f32 rotation{}; // In radians (Euler YXZ)
	};

	struct PrefabUUIDComponent
	{
		u64 uuid;
	};
}