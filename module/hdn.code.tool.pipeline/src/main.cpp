#include "core/core.h"
#include "core/stl/unordered_map.h"

#include "hobj/hobj_registry.h"
#include "hobj/hobj_source_filesystem.h"

#include "core/config/config.h"

#include "args.h"
#include <iostream>

#include "srcdb/cache.h"
#include "srcdb/symdb.h"
#include "srcdb/buildconfig/buildconfig.h"

int main(int argc, char *argv[])
{
	using namespace hdn;
	log_init();
	if (!args_init(argc, argv))
	{
		return 1;
	}
	config_init();
	cache_init();
	signature_init();

	HINFO("Pipeline started");

	const FPipelineCmdArgs &args = args_get();

	std::string dataModulePath = config_get_config_variable(CONFIG_SECTION_DATA, CONFIG_KEY_DATA_MODULE_PATH, "");

	auto start = std::chrono::high_resolution_clock::now();
	symdb_explore_sources(dataModulePath);
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	std::cout << "symdb_explore_sources took " << duration.count() << " nanoseconds.";

	Signature<XBuildConfigAsset> sig = make_signature<XBuildConfigAsset>("dreamlike_pc.buildconfig");
	Handle<XBuildConfigAsset> buildconfig = buildconfig_get(sig);

	return 0;
}
