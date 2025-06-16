#include "core/core.h"
#include "core/stl/unordered_map.h"

#include "hobj/scene/scene_hobj.h"
#include "hobj/hobj_registry.h"
#include "hobj/hobj_source_filesystem.h"

#include "config/config.h"

#include "cache.h"
#include "args.h"

#include "hobj/prefab/prefab_parse.h"
#include "hobj/prefab/prefab_hobj.h"

namespace hdn
{
	inline static constexpr const char* SOURCE_EXTENSION_DEFINITION = ".def";
	inline static constexpr const char* SOURCE_EXTENSION_PREFAB = ".pfb";
	inline static constexpr const char* SOURCE_EXTENSION_SHADER_FRAG = ".frag";
	inline static constexpr const char* SOURCE_EXTENSION_SHADER_VERT = ".vert";
	inline static constexpr const char* SOURCE_EXTENSION_FBX = ".fbx";
	inline static constexpr const char* SOURCE_EXTENSION_OBJ = ".obj";


	void hobj_registry_init()
	{
		HObjectRegistry& registry = HObjectRegistry::get();
		std::string cachePath = Configuration::get().get_root_config_variable(CONFIG_SECTION_PIPELINE, CONFIG_KEY_CACHE_PATH, "");
		registry.add_source<FilesystemObjectSource>("local", cachePath);
		registry.populate();
	}

	// TODO: Move into sourcedb
	// The high level source type supported by the pipeline
	enum class SourceType
	{
		UNKNOWN,
		DEFINITION, // Most common format for engine-specific assets (xml-like)
		PREFAB, // Format for structuring entities (xml-like)
		SHADER, // hlsl shader sources
		FBX, // fbx 3d model source
		OBJ, // obj 3d model source
		COUNT
	};

	static constexpr const char* s_SourceTypeStr[underlying(SourceType::COUNT)] = {
		"UNKNOWN",
		"DEFINITION",
		"PREFAB",
		"SHADER",
		"FBX",
		"OBJ"
	};

	struct Source
	{
		SourceType type;
		string path;
		// string name; // The given name to a source, depending on the source the algorithm to determine the name will differ.
	};

	const char* source_type_to_string(SourceType type)
	{
		return s_SourceTypeStr[underlying(type)];
	}

	SourceType get_source_type(const fspath& path)
	{
		if (filesystem_has_extension(path, SOURCE_EXTENSION_DEFINITION))
		{
			return SourceType::DEFINITION;
		}
		else if (filesystem_has_extension(path, SOURCE_EXTENSION_PREFAB))
		{
			return SourceType::PREFAB;
		}
		else if (filesystem_has_extension(path, SOURCE_EXTENSION_SHADER_FRAG) || filesystem_has_extension(path, SOURCE_EXTENSION_SHADER_VERT))
		{
			return SourceType::SHADER;
		}
		else if (filesystem_has_extension(path, SOURCE_EXTENSION_FBX))
		{
			return SourceType::FBX;
		}
		else if (filesystem_has_extension(path, SOURCE_EXTENSION_OBJ))
		{
			return SourceType::OBJ;
		}
		return SourceType::UNKNOWN;
	}

	void explore_source(const fspath& path)
	{
		if (!filesystem_is_directory(path))
		{
			SourceType type = get_source_type(path);
			if (type != SourceType::UNKNOWN)
			{
				HINFO("Source '{0}' found: {1}", path.string().c_str(), source_type_to_string(type));
			}
		}
	}

	void explore_sources(const fspath& path)
	{
		filesystem_iterate(path, explore_source, true);
	}
}

int main(int argc, char* argv[])
{
	using namespace hdn;
	log_init();
	if (!args_init(argc, argv))
	{
		return 1;
	}
	cache_init();
	hobj_registry_init();

	HINFO("Pipeline started");

	std::string dataModulePath = Configuration::get().get_root_config_variable(CONFIG_SECTION_DATA, CONFIG_KEY_DATA_MODULE_PATH, "");
	explore_sources(dataModulePath);

	const PipelineCmdArgs& args = args_get();

	std::string scenePath = fmt::format("{0}\\{1}", dataModulePath, args.scenePath);

	Prefab prefab;
	prefab_parse(prefab, scenePath);

	{
		HRef<HScene> hScene = HObjectRegistry::get().new_object<HScene>("scene_01");
		scene_set_world(hScene->get_scene(), prefab.world);
		HObjectFilesystemData data;
		data.path = "scene_01.hobj";
		HObjectRegistry::get().object_save(hScene.get(), "local", &data, sizeof(data));
	}

	{
		HRef<HPrefab> hPrefab = HObjectRegistry::get().new_object<HPrefab>("scene_01_prefab");
		prefab_set_world(hPrefab->get_prefab(), prefab.world);
		HObjectFilesystemData data;
		data.path = "scene_01_prefab.hobj";
		HObjectRegistry::get().object_save(hPrefab.get(), "local", &data, sizeof(data));
	}

	return 0;
}