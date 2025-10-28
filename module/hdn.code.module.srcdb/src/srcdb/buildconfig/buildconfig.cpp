#include "buildconfig.h"
#include "srcdb/cache.h"

namespace hdn
{
	static constexpr u64 BUILDCONFIG_CODE_VERSION = 1; // Initial buildconfig version

	Platform buildconfig_str_to_platform(const char *platform)
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

	static auto create_meta(flatbuffers::FlatBufferBuilder &builder, const SourceContext &ctx)
	{
		// Create file dependencies
		hdn::FileDependencyBuilder fd1(builder);
		std::string filePathStr = ctx.path.string();
		const char *filePath = filePathStr.c_str();
		fd1.add_path_hash(hash_generate(filePath, strlen(filePath)));
		fd1.add_last_modified_timestamp(filesystem_last_write_time(ctx.path));
		auto fd_offset1 = fd1.Finish();

		flatbuffers::Offset<hdn::FileDependency> file_dep_array[] = {fd_offset1};
		auto fbFileDeps = builder.CreateVector(file_dep_array, 1);

		// Build Metadata
		return hdn::CreateMetadata(builder, fbFileDeps, BUILDCONFIG_CODE_VERSION);
	}

	static bool buildconfig_parse(flatbuffers::FlatBufferBuilder &builder, const pugi::xml_node &node, const SourceContext &ctx)
	{
		// Meta
		auto fbMeta = create_meta(builder, ctx);

		// Name & id
		const pugi::char_t *nameStr = node.attribute("name").as_string();
		HASSERT(nameStr, "xsymbol require a name");
		uint64_t id = static_cast<uint64_t>(hash_generate(nameStr, strlen(nameStr)));
		auto fbName = builder.CreateString(nameStr);

		// Platform
		auto nPlatform = node.child("Platform");
		HASSERT(nPlatform, "buildconfig symbol require a platform node");
		const pugi::char_t *platformStr = nPlatform.attribute("platform").as_string();
		Platform platform = buildconfig_str_to_platform(platformStr);

		// Feature iteration
		std::vector<flatbuffers::Offset<flatbuffers::String>> fbFeaturesVector = {};
		auto nFeatures = node.child("Features");
		if (nFeatures)
		{
			for (const auto &nFeature : nFeatures.children("Feature"))
			{
				const pugi::char_t *sceneSymbolStr = nFeature.attribute("scene").as_string();
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
		cache_create_entry(id, builder.GetBufferPointer(), builder.GetSize(), true);
		return true;
	}

	bool buildconfig_parse_callback(const pugi::xml_node &node, const SourceContext &ctx)
	{
		flatbuffers::FlatBufferBuilder builder(2048);
		buildconfig_parse(builder, node, ctx);
		return false;
	}

	void buildconfig_get(const char *name, BuildConfigAsset *builconfig)
	{
		uint64_t object_id = static_cast<uint64_t>(hash_generate(name, strlen(name)));
		cache_fetch(object_id, reinterpret_cast<char *>(builconfig));
	}
}
