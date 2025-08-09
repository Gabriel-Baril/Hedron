#pragma once
#include "core/core.h"

namespace hdn
{
	struct FPipelineCmdArgs
	{
		std::string buildConfigPath;
	};

	bool args_init(int argc, char* argv[]);
	const FPipelineCmdArgs& args_get();
}