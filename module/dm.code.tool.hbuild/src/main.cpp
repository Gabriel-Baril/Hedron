#include "core/core.h"
#include "core/stl/unordered_map.h"
#include "core/config/config.h"

#include "pipeline/cache.h"
#include "pipeline/symdb.h"
#include "pipeline/strdb.h"
#include "pipeline/buildconfig/buildconfig.h"
#include "pipeline/request.h"

#include "args.h"

void test_request()
{
	using namespace dm;

	auto start = std::chrono::high_resolution_clock::now();
	Request<BuildConfigReadRequest> req("dreamlike_pc.buildconfig");
	Response<BuildConfigReadRequest> buildConfig = request_send(req);
	char buffer[512];
	request_get_slug(req, buffer, 512);
	DM_INFO_LOG("Request -> {0}", buffer);
	DM_INFO_LOG("Platform: {0}", (int)buildConfig.data->platform());
	const auto *vec = buildConfig.data->features();
	if (vec)
	{
		for (flatbuffers::uoffset_t i = 0; i < vec->size(); ++i)
		{
			const char *featureName = vec->Get(i)->c_str();
			if (featureName)
			{
				DM_INFO_LOG("Feature[{0}]: {1}", i, featureName);
			}
		}
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	DM_INFO_LOG("test_request took {0} nanoseconds.", duration.count());
}

void test_request_noprint()
{
	using namespace dm;
	auto start = std::chrono::high_resolution_clock::now();
	Request<BuildConfigReadRequest> req("dreamlike_pc.buildconfig");
	Response<BuildConfigReadRequest> buildConfig = request_send(req);
	DM_INFO_LOG("Platform: {0}", (int)buildConfig.data->platform());
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	DM_INFO_LOG("test_request_noprint took {0} nanoseconds.", duration.count());
}

int main(int argc, char *argv[])
{
	using namespace dm;
	log_init();
	if (!args_init(argc, argv))
	{
		return 1;
	}
	config_init();
	cache_init();
	signature_init();
	strdb_init();

	DM_INFO_LOG("hbuild started");

	const HBuildCmdArgs &args = args_get();

	std::string dataModulePath = config_get_config_variable(CONFIG_SECTION_DATA, CONFIG_KEY_DATA_MODULE_PATH, "");

	auto start = std::chrono::high_resolution_clock::now();
	symdb_explore_sources(dataModulePath);
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	DM_INFO_LOG("symdb_explore_sources took {0} nanoseconds.", duration.count());

	test_request();
	test_request_noprint();

	return 0;
}
