#include "core/core.h"
#include "core/core_filesystem.h"
#include "core/stl/vector.h"

#include "config/config.h"

namespace hdn
{
	bool predicate(const fspath& path)
	{
		auto s = FileSystem::Filename(path).string();
		HWARN(s.c_str());
		return Str_Equals(s.c_str(), ".module");
	}
}

int main()
{
	using namespace hdn;
	Log_Init();

	fspath rootConfig = Configuration::GetRootConfigPath();
	HINFO("HDN_ROOT={0}", rootConfig.string().c_str());

	string testSolution = Configuration::Get().GetRootConfigVariable("test", "ExecutableListFilePath", "");
	HINFO(testSolution.c_str());

	string moduleFolder = Configuration::Get().GetRootConfigVariable("path", "ModuleFolder", "");

	HINFO("Module Folder -> '{0}'", moduleFolder.c_str());
	
	fspath corePath = "hdn.code.module.core";

	try {
		vector<fspath> paths = FileSystem::Walk(moduleFolder, hdn::predicate, true);
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