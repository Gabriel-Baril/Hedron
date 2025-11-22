#pragma once

#include "pugixml/pugixml.hpp"

#include "core/core.h"
#include "core/hash.h"

#include "pipeline/symdb.h"
#include "pipeline/xsrc.h"

namespace hdn
{
	bool xasset_parse_feature(const pugi::xml_node &symbolNode, const SourceContext &ctx);
}
