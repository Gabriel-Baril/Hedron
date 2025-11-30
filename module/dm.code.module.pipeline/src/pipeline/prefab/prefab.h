#pragma once

#include "flecs/flecs.h"

#include "core/core.h"
#include "core/stl/vector.h"
#include "core/io/stream_write.h"
#include "core/io/stream_read.h"

#include "pipeline/symdb.h"

namespace dm
{
	struct SPrefabDef
	{
		sym_t name;
		flecs::world world;
	};

	void prefab_set_world(SPrefabDef &prefab, flecs::world world);
}
