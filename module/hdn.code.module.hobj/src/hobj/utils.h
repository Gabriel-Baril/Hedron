#pragma once
#include <functional>
#include "core/core.h"
#include "config/config.h"
#include "xxhash/xxhash.h"

namespace hdn
{
	string get_data_path(const string& relativeDataPath);
	u64 str_uuid_to_u64(const char* uuid);
}