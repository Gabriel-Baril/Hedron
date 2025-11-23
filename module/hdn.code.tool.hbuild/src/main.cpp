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
	using namespace hdn;

	auto start = std::chrono::high_resolution_clock::now();
	Request<BuildConfigGetRequest> req("dreamlike_pc.buildconfig");
	Response<BuildConfigGetRequest> buildConfig = request_send(req);
	char buffer[512];
	request_get_slug(req, buffer, 512);
	HINFO("Request -> {0}", buffer);
	HINFO("Platform: {0}", (int)buildConfig.data->platform());
	const auto* vec = buildConfig.data->features();
	if (vec)
	{
		for (flatbuffers::uoffset_t i = 0; i < vec->size(); ++i)
		{
			const char* featureName = vec->Get(i)->c_str();
			if (featureName)
			{
				HINFO("Feature[{0}]: {1}", i, featureName);
			}
		}
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	HINFO("test_request took {0} nanoseconds.", duration.count());
}

void test_request_noprint()
{
	using namespace hdn;

	auto start = std::chrono::high_resolution_clock::now();
	Request<BuildConfigGetRequest> req("dreamlike_pc.buildconfig");
	Response<BuildConfigGetRequest> buildConfig = request_send(req);
	HINFO("Platform: {0}", (int)buildConfig.data->platform());
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	HINFO("test_request_noprint took {0} nanoseconds.", duration.count());
}

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
	strdb_init();

	HINFO("hbuild started");

	const HBuildCmdArgs &args = args_get();

	std::string dataModulePath = config_get_config_variable(CONFIG_SECTION_DATA, CONFIG_KEY_DATA_MODULE_PATH, "");

	auto start = std::chrono::high_resolution_clock::now();
	symdb_explore_sources(dataModulePath);
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	HINFO("symdb_explore_sources took {0} nanoseconds.", duration.count());

	test_request();
	test_request_noprint();

	return 0;
}
