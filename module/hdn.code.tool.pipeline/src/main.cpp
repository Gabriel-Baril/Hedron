#include "core/core.h"
#include "core/stl/unordered_map.h"

#include "hobj/hobj_registry.h"
#include "hobj/hobj_source_filesystem.h"

#include "hobj/config/config.h"

#include "cache.h"
#include "args.h"

#include "symdb.h"

#include <iostream>

#include "buildconfig/buildconfig.h"

namespace hdn
{
	void hobj_registry_init()
	{
		std::string cachePath = config_get_root_config_variable(CONFIG_SECTION_PIPELINE, CONFIG_KEY_CACHE_PATH, "");
		HObjectRegistry::add_source<FilesystemObjectSource>("local", cachePath);
		HObjectRegistry::populate();
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
	config_init();
	hobj_registry_init();
	cache_init();

	HINFO("Pipeline started");

	const FPipelineCmdArgs& args = args_get();
	
	std::string dataModulePath = config_get_root_config_variable(CONFIG_SECTION_DATA, CONFIG_KEY_DATA_MODULE_PATH, "");

	auto start = std::chrono::high_resolution_clock::now();
	symdb_explore_sources(dataModulePath);
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	std::cout << "symdb_explore_sources took {0} nanoseconds: " << duration.count();

	return 0;
}