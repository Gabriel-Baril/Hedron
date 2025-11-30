#include "args.h"

#include "CLI/CLI.hpp"

namespace dm
{
	static HBuildCmdArgs s_PipelineCmdArgs;

	bool args_init(int argc, char *argv[])
	{
		CLI::App app{"pipeline"};
		app.add_option("-b,--buildconfig", s_PipelineCmdArgs.buildConfigPath, "Build Config Path");
		try
		{
			app.parse(argc, argv);
			return true;
		}
		catch (const CLI::ParseError &e)
		{
			DM_CRITICAL_LOG("Error parsing cmd args: {0}", e.what());
			return false;
		}
	}

	const HBuildCmdArgs &args_get()
	{
		return s_PipelineCmdArgs;
	}
}
