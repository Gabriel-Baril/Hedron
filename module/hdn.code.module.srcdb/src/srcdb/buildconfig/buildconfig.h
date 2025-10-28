#pragma once
#include "core/core.h"
#include "core/hash.h"
#include "core/stl/vector.h"

#include "pugixml/pugixml.hpp"

#include "srcdb/symdb.h"
#include "srcdb/xsrc.h"
#include "srcdb/generated/buildconfig_generated.h"

namespace hdn
{
	bool buildconfig_parse_callback(const pugi::xml_node& symbolNode, const SourceContext& ctx);
	void buildconfig_get(const char* name, BuildConfigAsset* builconfig);
}