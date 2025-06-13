#pragma once

#include "core/core.h"
#include "entity_component_parse.h"

namespace hdn
{
	struct HierarchyComponent
	{
		int dummy;
	};

	bool component_parse_hierarchy(pugi::xml_node componentNode, EntityDef& entDef);
}