#pragma once

#include "pugixml/pugixml.hpp"

#include "flecs/flecs.h"

#include "core/core.h"

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