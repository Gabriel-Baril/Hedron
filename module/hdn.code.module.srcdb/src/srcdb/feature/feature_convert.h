#pragma once

#include "core/core.h"
#include "core/hash.h"

#include "srcdb/symdb.h"
#include "srcdb/xsrc.h"

#include "pugixml/pugixml.hpp"

namespace hdn
{
	bool feature_parse_callback(const pugi::xml_node& symbolNode, const SourceContext& ctx);
}