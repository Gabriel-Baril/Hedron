#pragma once

#include "core/core.h"
#include "flecs/flecs.h"
#include "pugixml/pugixml.hpp"

namespace hdn
{
	enum class EntityComponentType
	{
		TRANSFORM,
		MODEL,
		HIERARCHY,
		COUNT,

		UNKNOWN
	};

	struct EntityDef
	{
		u64 uuid;
		flecs::entity entity;

		template<typename T>
		void add_component(T comp)
		{
			entity.set(comp);
		}
	};

	bool entity_parse_component(pugi::xml_node componentNode, EntityDef& entDef);
}