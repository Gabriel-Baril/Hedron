#pragma once

#include "core/core.h"
#include "prefab_component_parse.h"

namespace hdn
{
	struct PrefabModelComponent
	{
		int dummy;
	};

	bool component_parse_model(pugi::xml_node componentNode, flecs::entity ent);
}