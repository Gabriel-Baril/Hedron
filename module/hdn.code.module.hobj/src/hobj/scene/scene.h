#pragma once

#include "core/core.h"
#include "core/stl/vector.h"
#include "core/io/stream_write.h"
#include "core/io/stream_read.h"

#include "scene_entity.h"

namespace hdn
{
	struct SceneDef
	{
		int maxLight;
		int maxPrimaryLight;
		vector<SceneEntityDef> entities;
	};

	void scene_def_set_max_light(SceneDef& object, int maxLight);
	void scene_def_set_max_primary_light(SceneDef& object, int maxPrimaryLight);
	void scene_def_emplace_entity(SceneDef& object, const SceneEntityDef& entity);

	void scene_def_serialize(hostream& stream, const SceneDef& object);
	void scene_def_deserialize(histream& stream, SceneDef& object);
}