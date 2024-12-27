#include "core/core.h"
#include "core/core_filesystem.h"
#include "config/config.h"

namespace hdn
{
	bool Predicate(const fspath& path)
	{
		auto s = FileSystem::Filename(path).string();
		HWARN(s.c_str());
		return Str_Equals(s.c_str(), ".module");
	}
}

int main()
{
	using namespace hdn;
#if USING(ENABLE_LOG)
	Log_Init();
#endif

	std::filesystem::path rootConfig = Configuration::GetRootConfigPath();
	HINFO("HDN_ROOT={0}", rootConfig.string().c_str());

	std::string testSolution = Configuration::Get().GetRootConfigVariable("test", "ExecutableListFilePath", "");
	HINFO(testSolution.c_str());

	std::string moduleFolder = Configuration::Get().GetRootConfigVariable("path", "ModuleFolder", "");

	HINFO("Module Folder -> '{0}'", moduleFolder.c_str());
	
	fspath corePath = "hdn.code.module.core";

	try {
		TVector<fspath> paths = FileSystem::Walk(moduleFolder, hdn::Predicate, true);
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