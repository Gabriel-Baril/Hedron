#pragma once
#include "core/core.h"
#include "core/hash.h"
#include "core/stl/vector.h"

#include "symdb.h"
#include "pugixml/pugixml.hpp"

namespace hdn
{
	struct PipelineContext
	{
	};

	namespace buildconfig
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
	}

	// Agnostic buildconfig
	struct SBuildConfigDef
	{
		Symbol name = SYMNULL;
		Symbol parent = SYMNULL;
		buildconfig::EPlatform platform = buildconfig::EPlatform::UNKNOWN;
		vector<Variant<buildconfig::Scene>> scenes;
	};

	// Contextualized buildconfig
	struct SBuildConfig
	{
		Symbol name = SYMNULL;
		buildconfig::EPlatform platform = buildconfig::EPlatform::UNKNOWN;
		vector<buildconfig::Scene> scenes;
	};

	bool buildconfig_contextualize(const PipelineContext& ctx, const SBuildConfigDef& def, SBuildConfig& out);
	bool buildconfig_parse_callback(const pugi::xml_node& symbolNode);
}