#include "xasset_parse.h"

#if USING(SYM_BUILDCONFIG)
#include "pugixml/pugixml.hpp"

#include "core/hash.h"
#include "core/stl/vector.h"

#include "pipeline/symdb.h"
#include "pipeline/xsrc.h"
#include "pipeline/generated/buildconfig_generated.h"
#include "pipeline/fbs_util.h"
#include "pipeline/buildconfig/xasset.h"

namespace hdn
{
	CPlatform buildconfig_str_to_platform(const char *platform)
	{
		char lowerCaseBuffer[SYMBOL_ATTRIBUTE_VALUE_MAX_LENGTH];
		str_copy(lowerCaseBuffer, platform);
		str_to_lowercase(lowerCaseBuffer, strlen(lowerCaseBuffer));
		for (int i = 0; i < underlying(CPlatform::CPlatform_MAX) + 1; i++)
		{
			char lowerCaseBufferPlatform[SYMBOL_ATTRIBUTE_VALUE_MAX_LENGTH];
			str_copy(lowerCaseBufferPlatform, EnumNamesCPlatform()[i]);
			str_to_lowercase(lowerCaseBufferPlatform, strlen(lowerCaseBufferPlatform));
			if (str_equals(lowerCaseBuffer, lowerCaseBufferPlatform))
			{
				return static_cast<CPlatform>(i);
			}
		}
		return CPlatform::CPlatform_UNKNOWN;
	}

	static bool buildconfig_parse(flatbuffers::FlatBufferBuilder &builder, const pugi::xml_node &node, const SourceContext &ctx)
	{
		// Name & id
		const char *nameStr = get_xml_attr(node, "name");
		HASSERT(nameStr, "xsymbol require a name");
		auto fbName = builder.CreateString(nameStr);

		// CPlatform
		auto nPlatform = node.child("Platform");
		HASSERT(nPlatform, "buildconfig symbol require a platform node");
		const pugi::char_t *platformStr = nPlatform.attribute("platform").as_string();
		CPlatform platform = buildconfig_str_to_platform(platformStr);

		// Feature iteration
		std::vector<flatbuffers::Offset<flatbuffers::String>> fbFeaturesVector = {};
		auto nFeatures = node.child("Features");
		if (nFeatures)
		{
			for (const auto &nFeature : nFeatures.children("Feature"))
			{
				const pugi::char_t *featureSymbolStr = nFeature.attribute("feature").as_string();
				HASSERT(featureSymbolStr, "scene attibute cannot be ommited");
				auto fbFeatureName = builder.CreateString(featureSymbolStr);
				fbFeaturesVector.emplace_back(fbFeatureName);
			}
		}
		auto fbFeatures = builder.CreateVector(fbFeaturesVector);

		Signature<XBuildConfigAssetObject> sig(nameStr);
		const u64 oId = object_get_id(sig);

		auto buildConfig = CreateXBuildConfigAsset(
				builder,
				oId,
				fbName,
				platform,
				fbFeatures);

		FinishXBuildConfigAssetBuffer(builder, buildConfig);

		BeginObjectInfo info;
		info.totalPathDep = 1;
		info.totalObjDep = 0;
		bool ok = cache_obj_begin(oId, info);
		if (ok)
		{
			cache_obj_pathdep(oId, ctx.path); // TODO: Once xsym parenting will exist, this logic will get more complex (if we want parent to be in different xsrc)
			cache_obj_payload(oId, builder.GetBufferPointer(), builder.GetSize());
			cache_obj_end(oId);
			cache_obj_save(oId);
		}

		return ok;
	}

	bool xasset_parse_buildconfig(const pugi::xml_node &node, const SourceContext &ctx)
	{
		flatbuffers::FlatBufferBuilder builder(2048);
		return buildconfig_parse(builder, node, ctx);
	}
}
#endif
