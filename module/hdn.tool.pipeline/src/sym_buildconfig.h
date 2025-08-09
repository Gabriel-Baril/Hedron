#pragma once
#include "core/core.h"
#include "core/hash.h"
#include "core/stl/vector.h"

#include "symdb.h"
#include "pugixml/pugixml.hpp"

namespace hdn
{
	enum class EBuildConfigPlatform
	{
		PC,
		COUNT,

		UNKNOWN
	};
	EBuildConfigPlatform buildconfig_str_to_platform(const char* platform);

	// Data flagged as variant can be impacted by context
	template<typename T>
	struct Variant
	{
		T node;
		Symbol variant = NULL_SYMBOL;
	};

	struct SBuildConfigDef
	{
		struct Scene
		{
			Symbol sceneSymbol;
		};

		Symbol name;
		EBuildConfigPlatform platform;
		vector<Variant<Scene>> scenes;
	};

	bool buildconfig_parse_callback(const pugi::xml_node& symbolNode);
}