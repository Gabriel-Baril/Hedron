#pragma once
#include <functional>
#include "core/core.h"
#include "core/config/config.h"
#include "xxhash/xxhash.h"

namespace dm
{
	string get_data_path(const string &relativeDataPath);
}
