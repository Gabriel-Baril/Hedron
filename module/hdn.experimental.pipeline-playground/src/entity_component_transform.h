#pragma once

#include "core/core.h"
#include "entity_component_parse.h"

namespace hdn
{
	struct TransformComponent
	{
		vec3f32 position;
		vec3f32 rotation;
		vec3f32 scale;
	};

	bool component_parse_transform(pugi::xml_node componentNode, EntityDef& entDef);
}