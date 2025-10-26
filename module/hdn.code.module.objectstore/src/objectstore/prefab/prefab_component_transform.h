#pragma once

#include "core/core.h"
#include "prefab_component_parse.h"

namespace hdn
{
	struct PrefabTransformComponent
	{
		vec3f32 position{};
		vec3f32 scale{ 1.0f, 1.0f, 1.0f };
		vec3f32 rotation{}; // In radians (Euler YXZ)
	};

	bool component_parse_transform(pugi::xml_node componentNode, flecs::entity ent);
}