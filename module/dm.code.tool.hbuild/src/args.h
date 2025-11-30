#pragma once

#include "core/core.h"

namespace dm
{
	struct HBuildCmdArgs
	{
		std::string buildConfigPath;
	};

	bool args_init(int argc, char *argv[]);
	const HBuildCmdArgs &args_get();
}
