#pragma once

#include "core/core_define.h"

#if USING(DM_SYM_BUILDCONFIG)
#include "pugixml/pugixml.hpp"

#include "core/core.h"
#include "core/hash.h"

#include "pipeline/symdb.h"
#include "pipeline/xsrc.h"

namespace dm
{
	bool xasset_parse_buildconfig(const pugi::xml_node &symbolNode, const SourceContext &ctx);
}
#endif
