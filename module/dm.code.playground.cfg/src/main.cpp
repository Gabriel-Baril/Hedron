#include "core/core.h"
#include "core/core_filesystem.h"
#include "core/stl/vector.h"

#include "core/config/config.h"

namespace dm
{
	bool predicate(const fspath &path)
	{
		auto s = filesystem_filename(path).string();
		DM_WARNING_LOG(s.c_str());
		return str_equals(s.c_str(), ".module");
	}
}

int main()
{
	using namespace dm;
	log_init();

	fspath rootConfig = Configuration::get_root_config_path();
	DM_INFO_LOG("DM_ROOT={0}", rootConfig.string().c_str());

	string testSolution = Configuration::get().get_root_config_variable(CONFIG_SECTION_TEST, CONFIG_KEY_EXECUTABLE_LIST_PATH, "");
	DM_INFO_LOG(testSolution.c_str());

	string moduleFolder = Configuration::get().get_root_config_variable(CONFIG_SECTION_PATH, CONFIG_KEY_MODULE_FOLDER_PATH, "");

	DM_INFO_LOG("Module Folder -> '{0}'", moduleFolder.c_str());

	fspath corePath = "dm.code.module.core";

	try
	{
		vector<fspath> paths = filesystem_walk(moduleFolder, dm::predicate, true);
		for (const auto &path : paths)
		{
			DM_INFO_LOG("{0}", path.filename().string().c_str());
		}
	}
	catch (const std::exception &e)
	{
		DM_ERROR_LOG("FS error {0}", e.what());
	}
}
