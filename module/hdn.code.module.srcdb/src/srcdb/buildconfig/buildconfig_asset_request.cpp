#include "buildconfig_asset_request.h"

#include "core/hash.h"
#include "core/stl/vector.h"

#include "pugixml/pugixml.hpp"

#include "srcdb/symdb.h"
#include "srcdb/xsrc.h"
#include "srcdb/generated/buildconfig_generated.h"
#include "srcdb/fbs_util.h"
#include "buildconfig_asset.h"

namespace hdn
{
	Platform buildconfig_str_to_platform(const char* platform)
	{
		char lowerCaseBuffer[SYMBOL_ATTRIBUTE_VALUE_MAX_LENGTH];
		str_copy(lowerCaseBuffer, platform);
		str_to_lowercase(lowerCaseBuffer, strlen(lowerCaseBuffer));
		for (int i = 0; i < underlying(Platform::Platform_MAX); i++)
		{
			if (str_equals(lowerCaseBuffer, EnumNamesPlatform()[i]))
			{
				return static_cast<Platform>(i);
			}
		}
		return Platform::Platform_UNKNOWN;
	}

	static bool buildconfig_parse(flatbuffers::FlatBufferBuilder& builder, const pugi::xml_node& node, const SourceContext& ctx)
	{
		// Meta
		auto fbMeta = create_meta(builder, ctx, BUILDCONFIG_ASSET_CODE_VERSION);

		// Name & id
		const pugi::char_t* nameStr = node.attribute("name").as_string();
		HASSERT(nameStr, "xsymbol require a name");
		uint64_t id = static_cast<uint64_t>(hash_generate(nameStr, strlen(nameStr)));
		auto fbName = builder.CreateString(nameStr);

		// Platform
		auto nPlatform = node.child("Platform");
		HASSERT(nPlatform, "buildconfig symbol require a platform node");
		const pugi::char_t* platformStr = nPlatform.attribute("platform").as_string();
		Platform platform = buildconfig_str_to_platform(platformStr);

		// Feature iteration
		std::vector<flatbuffers::Offset<flatbuffers::String>> fbFeaturesVector = {};
		auto nFeatures = node.child("Features");
		if (nFeatures)
		{
			for (const auto& nFeature : nFeatures.children("Feature"))
			{
				const pugi::char_t* sceneSymbolStr = nFeature.attribute("scene").as_string();
				HASSERT(sceneSymbolStr, "scene attibute cannot be ommited");
				auto fbFeatureName = builder.CreateString(sceneSymbolStr);
				fbFeaturesVector.emplace_back(fbFeatureName);
			}
		}
		auto fbFeatures = builder.CreateVector(fbFeaturesVector);

		auto buildConfig = CreateBuildConfigAsset(
			builder,
			fbMeta,
			id,
			fbName,
			platform,
			fbFeatures);

		FinishBuildConfigAssetBuffer(builder, buildConfig);
		cache_obj_store(id, builder.GetBufferPointer(), builder.GetSize());
		return true;
	}

	bool buildconfig_asset_xsrc_parse(const pugi::xml_node& node, const SourceContext& ctx)
	{
		flatbuffers::FlatBufferBuilder builder(2048);
		buildconfig_parse(builder, node, ctx);
		return false;
	}
}