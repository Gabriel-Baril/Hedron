#include "core/core.h"
#include "core/core_filesystem.h"
#include "core/stl/vector.h"

#include "config/config.h"

namespace hdn
{
	bool predicate(const fspath& path)
	{
		auto s = filesystem_filename(path).string();
		HWARN(s.c_str());
		return str_equals(s.c_str(), ".module");
	}
}

int main()
{
	using namespace hdn;
	log_init();

	fspath rootConfig = Configuration::get_root_config_path();
	HINFO("HDN_ROOT={0}", rootConfig.string().c_str());

	string testSolution = Configuration::get().get_root_config_variable("test", "ExecutableListFilePath", "");
	HINFO(testSolution.c_str());

	string moduleFolder = Configuration::get().get_root_config_variable("path", "ModuleFolder", "");

	HINFO("Module Folder -> '{0}'", moduleFolder.c_str());
	
	fspath corePath = "hdn.code.module.core";

	try {
		vector<fspath> paths = filesystem_walk(moduleFolder, hdn::predicate, true);
		for (const auto& path : paths)
		{
			HINFO("{0}", path.filename().string().c_str());
		}
	}
	catch (const std::exception& e)
	{
		HERR("FS error {0}", e.what());
	}
}