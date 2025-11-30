#pragma once

#include "core/core.h"
#include "core/core_filesystem.h"

namespace dm
{
	struct SourceContext
	{
		fspath path;
	};

	bool xsrc_agnostic_parse(const fspath &path);
}
