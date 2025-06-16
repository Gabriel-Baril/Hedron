#pragma once

#include "core/core.h"
#include "core/stl/vector.h"
#include "core/io/stream_write.h"
#include "core/io/stream_read.h"

#include "flecs/flecs.h"

namespace hdn
{
	struct Prefab
	{
		flecs::world world;
	};

	void prefab_set_world(Prefab& prefab, flecs::world world);

	void prefab_serialize(hostream& stream, const Prefab& object);
	void prefab_deserialize(histream& stream, Prefab& object);
}