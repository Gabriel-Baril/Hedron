#pragma once

#include "core/core.h"
#include "core/hash.h"

#include "srcdb/symdb.h"
#include "srcdb/xsrc.h"

#include "pugixml/pugixml.hpp"

namespace hdn
{
	bool buildconfig_asset_xsrc_parse(const pugi::xml_node& symbolNode, const SourceContext& ctx);
}