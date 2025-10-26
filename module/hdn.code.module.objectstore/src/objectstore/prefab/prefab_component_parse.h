#pragma once

#include "core/core.h"
#include "flecs/flecs.h"
#include "pugixml/pugixml.hpp"

namespace hdn
{
	enum class PrefabComponentType
	{
		UUID_COMPONENT,
		TRANSFORM_COMPONENT,
		MODEL_COMPONENT,
		HIERARCHY_COMPONENT,
		COUNT,

		UNKNOWN
	};

	bool entity_parse_component(pugi::xml_node componentNode, flecs::entity ent);
}