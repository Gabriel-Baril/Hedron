#pragma once

#include "core/core.h"
#include "core/hash.h"

#include "srcdb/symdb.h"
#include "srcdb/xsrc.h"

#include "pugixml/pugixml.hpp"

namespace hdn
{
	bool xasset_parse_feature(const pugi::xml_node& symbolNode, const SourceContext& ctx);
}