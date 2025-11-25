#include "core/core.h"
#include "core/core_filesystem.h"
#include "core/stl/vector.h"

#include "core/config/config.h"

namespace hdn
{
	bool predicate(const fspath& path)
	{
		auto s = filesystem_filename(path).string();
		HDN_WARNING_LOG(s.c_str());
		return str_equals(s.c_str(), ".module");
	}
}

int main()
{
	using namespace hdn;
	log_init();

	fspath rootConfig = Configuration::get_root_config_path();
	HDN_INFO_LOG("HDN_ROOT={0}", rootConfig.string().c_str());

	string testSolution = Configuration::get().get_root_config_variable(CONFIG_SECTION_TEST, CONFIG_KEY_EXECUTABLE_LIST_PATH, "");
	HDN_INFO_LOG(testSolution.c_str());

	string moduleFolder = Configuration::get().get_root_config_variable(CONFIG_SECTION_PATH, CONFIG_KEY_MODULE_FOLDER_PATH, "");

	HDN_INFO_LOG("Module Folder -> '{0}'", moduleFolder.c_str());
	
	fspath corePath = "hdn.code.module.core";

	try {
		vector<fspath> paths = filesystem_walk(moduleFolder, hdn::predicate, true);
		for (const auto& path : paths)
		{
			HDN_INFO_LOG("{0}", path.filename().string().c_str());
		}
	}
	catch (const std::exception& e)
	{
		HDN_ERROR_LOG("FS error {0}", e.what());
	}
}