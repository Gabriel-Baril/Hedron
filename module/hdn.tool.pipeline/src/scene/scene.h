#pragma once

#include "core/core.h"
#include "core/stl/vector.h"
#include "core/io/stream_write.h"
#include "core/io/stream_read.h"

#include "flecs/flecs.h"

namespace hdn
{
	struct Scene
	{
		flecs::world world;
	};

	void scene_set_world(Scene& scene, flecs::world world);

	void scene_serialize(hostream& stream, const Scene& object);
	void scene_deserialize(histream& stream, Scene& object);
}