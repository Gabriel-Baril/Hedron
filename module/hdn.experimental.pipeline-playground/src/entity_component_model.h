#pragma once

#include "core/core.h"
#include "entity_component_parse.h"

namespace hdn
{
	struct ModelComponent
	{
		int dummy;
	};

	bool component_parse_model(pugi::xml_node componentNode, EntityDef& entDef);
}