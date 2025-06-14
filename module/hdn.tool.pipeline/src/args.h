#pragma once
#include "core/core.h"

namespace hdn
{
	struct PipelineCmdArgs
	{
		std::string scenePath;
	};

	bool args_init(int argc, char* argv[]);
	const PipelineCmdArgs& args_get();
}