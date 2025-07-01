#include "args.h"

#include "CLI/CLI.hpp"

namespace hdn
{
	static FPipelineCmdArgs s_PipelineCmdArgs;

	bool args_init(int argc, char* argv[])
	{
		CLI::App app{ "pipeline" };
		app.add_option("-s,--scene", s_PipelineCmdArgs.scenePath, "Scene Path");
		try
		{
			app.parse(argc, argv);               
			return true;
		}
		catch (const CLI::ParseError& e)
		{
			HCRIT("Error parsing cmd args: {0}", e.what());
			return false;
		}
	}

	const FPipelineCmdArgs& args_get()
	{
		return s_PipelineCmdArgs;
	}
}

