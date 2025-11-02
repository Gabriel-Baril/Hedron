#pragma once

#include "core/core.h"
#include "inih/INIReader.h"
#include "core/stl/string.h"

namespace hdn
{
	string ini_get_variable(const INIReader& reader, const string& section, const string& name, const string& defaultValue);
}