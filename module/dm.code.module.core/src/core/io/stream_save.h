#pragma once

#include "core/core_filesystem.h"

namespace dm
{
	bool save_binary_to_file(const char *savePath, const void *data, u64 size);
}
