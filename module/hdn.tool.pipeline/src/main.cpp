#include "core/core.h"
#include "core/stl/unordered_map.h"

#include "hobj/scene/scene_hobj.h"
#include "hobj/hobj_registry.h"
#include "hobj/hobj_source_filesystem.h"

#include "core/config/config.h"

#include "cache.h"
#include "args.h"

#include "hobj/prefab/prefab_parse.h"
#include "hobj/prefab/prefab_hobj.h"

#include "srcdb.h"

namespace hdn
{
	void hobj_registry_init()
	{
		HObjectRegistry& registry = HObjectRegistry::get();
		std::string cachePath = Configuration::get().get_root_config_variable(CONFIG_SECTION_PIPELINE, CONFIG_KEY_CACHE_PATH, "");
		registry.add_source<FilesystemObjectSource>("local", cachePath);
		registry.populate();
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
	srcdb_explore_sources(dataModulePath);

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