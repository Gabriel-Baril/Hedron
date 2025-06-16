#pragma once
#include "core/core.h"
#include "core/core_filesystem.h"

#include "pugixml/pugixml.hpp"
#include "prefab.h"

namespace hdn
{
	bool prefab_parse(Prefab& prefab, const fspath& prefabPath);
}