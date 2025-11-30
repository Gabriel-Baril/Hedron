#pragma once
#include <functional>
#include "core/core.h"
#include "core/config/config.h"
#include "xxhash/xxhash.h"

namespace hdn
{
	string get_data_path(const string& relativeDataPath);
}