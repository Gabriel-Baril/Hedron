#include "core/core.h"
#include "core/stl/unordered_map.h"
#include "core/config/config.h"

#include "pipeline/cache.h"
#include "pipeline/symdb.h"
#include "pipeline/buildconfig/buildconfig.h"
#include "pipeline/request.h"

#include "args.h"

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

	HINFO("hbuild started");

	const HBuildCmdArgs &args = args_get();

	std::string dataModulePath = config_get_config_variable(CONFIG_SECTION_DATA, CONFIG_KEY_DATA_MODULE_PATH, "");

	auto start = std::chrono::high_resolution_clock::now();
	symdb_explore_sources(dataModulePath);
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	HINFO("symdb_explore_sources took {0} nanoseconds.", duration.count());

	Response<BuildConfigGetRequest> buildConfig = request_send<BuildConfigGetRequest>("dreamlike_pc.buildconfig");
	const auto *vec = buildConfig.data->features();
	if (vec)
	{
		for (flatbuffers::uoffset_t i = 0; i < vec->size(); ++i)
		{
			const char *featureName = vec->Get(i)->c_str();
			if (featureName)
			{
				HINFO("Feature[{0}]: {1}", i, featureName);
			}
		}
	}

	return 0;
}
