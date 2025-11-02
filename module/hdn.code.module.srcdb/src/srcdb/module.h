#pragma once
#include "core/core.h"

// A module refer to a folder in \module containing a .module file. This file offer reflection mechanisms on modules.

namespace hdn
{
	void module_init(const char* modulePath);
	void module_load(const char* name);
}