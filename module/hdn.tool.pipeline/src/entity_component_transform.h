#pragma once

#include "core/core.h"
#include "entity_component_parse.h"

namespace hdn
{
	bool component_parse_transform(pugi::xml_node componentNode, EntityDef& entDef);
}