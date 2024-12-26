#include "core/core.h"
#include "config/config.h"

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
}