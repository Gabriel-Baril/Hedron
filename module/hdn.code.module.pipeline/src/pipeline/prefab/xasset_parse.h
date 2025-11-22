#pragma once

#include "pugixml/pugixml.hpp"

#include "core/core.h"
#include "core/core_filesystem.h"

#include "prefab.h"
#include "pipeline/xsrc.h"

namespace hdn
{
	bool prefab_parse(SPrefabDef &def, const pugi::xml_node &node);
	bool xasset_parse_prefab(const pugi::xml_node &symbolNode, const SourceContext &ctx);
}
