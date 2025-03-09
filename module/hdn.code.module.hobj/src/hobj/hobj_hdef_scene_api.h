#pragma once

#include "core/core.h"
#include "core/stl/vector.h"
#include "core/io/hostream.h"
#include "core/io/histream.h"
#include "hobjsub_map_entity.h"

namespace hdn
{
	struct hobj_hdef_scene
	{
		int max_light;
		int max_primary_light;
		vector<hobjsub_map_entity> entities;
	};

	void hobj_hdef_scene_set_max_light(hobj_hdef_scene& object, int max_light);
	void hobj_hdef_scene_set_max_primary_light(hobj_hdef_scene& object, int max_primary_light);
	void hobj_hdef_scene_emplace_entity(hobj_hdef_scene& object, const hobjsub_map_entity& entity);

	void hobj_hdef_scene_serialize(hostream& stream, const hobj_hdef_scene& object);
	void hobj_hdef_scene_deserialize(histream& stream, hobj_hdef_scene& object);
}