#pragma once
#include "core/core.h"
#include "core/hash.h"
#include "core/stl/vector.h"

#include "symdb.h"
#include "pugixml/pugixml.hpp"

namespace hdn
{
	enum class EPlatform
	{
		PC,
		COUNT,

		UNKNOWN
	};
	EPlatform buildconfig_str_to_platform(const char* platform);

	struct Scene
	{
		Symbol sceneSymbol;
	};

	struct SBuildConfig
	{
		Symbol name = SYMNULL;
		EPlatform platform = EPlatform::UNKNOWN;
		vector<Scene> scenes;
	};

	bool buildconfig_parse_callback(const pugi::xml_node& symbolNode);
}