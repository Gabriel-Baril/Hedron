#pragma once

#include "core/core.h"
#include "prefab_component_parse.h"

namespace hdn
{
	struct PrefabHierarchyComponent
	{
		int dummy;
	};

	bool component_parse_hierarchy(pugi::xml_node componentNode, flecs::entity ent);
}