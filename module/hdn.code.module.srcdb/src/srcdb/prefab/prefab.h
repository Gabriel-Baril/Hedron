#pragma once

#include "core/core.h"
#include "core/stl/vector.h"
#include "core/io/stream_write.h"
#include "core/io/stream_read.h"

#include "flecs/flecs.h"
#include "srcdb/symdb.h"

namespace hdn
{
	struct SPrefabDef
	{
		sym_t name;
		flecs::world world;
	};

	void prefab_set_world(SPrefabDef& prefab, flecs::world world);
}