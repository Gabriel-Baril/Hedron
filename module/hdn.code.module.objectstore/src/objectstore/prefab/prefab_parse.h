#pragma once
#include "core/core.h"
#include "core/core_filesystem.h"

#include "pugixml/pugixml.hpp"
#include "prefab.h"

namespace hdn
{
	bool prefab_parse(SPrefabDef& def, const pugi::xml_node& node);
	bool prefab_parse_callback(const pugi::xml_node& symbolNode);
}