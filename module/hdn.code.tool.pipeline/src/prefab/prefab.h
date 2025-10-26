#pragma once

#include "core/core.h"
#include "core/stl/vector.h"
#include "core/io/stream_write.h"
#include "core/io/stream_read.h"

#include "flecs/flecs.h"
#include "symdb.h"

namespace hdn
{
	struct SPrefabDef
	{
		Symbol name;
		flecs::world world;
	};

	void prefab_set_world(SPrefabDef& prefab, flecs::world world);

	// Instead of having hostream at that stage use a normal struct
	void prefab_serialize(hostream& stream, const SPrefabDef& object);
	void prefab_deserialize(histream& stream, SPrefabDef& object);
}