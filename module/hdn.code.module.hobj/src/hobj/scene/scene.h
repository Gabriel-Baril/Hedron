#pragma once

#include "core/core.h"
#include "core/stl/vector.h"
#include "core/io/hostream.h"
#include "core/io/histream.h"

#include "scene_entity.h"

namespace hdn
{
	struct SceneDef
	{
		int maxLight;
		int maxPrimaryLight;
		vector<SceneEntityDef> entities;
	};

	void SceneDef_SetMaxLight(SceneDef& object, int maxLight);
	void SceneDef_SetMaxPrimaryLight(SceneDef& object, int maxPrimaryLight);
	void SceneDef_EmplaceEntity(SceneDef& object, const SceneEntityDef& entity);

	void SceneDef_Serialize(hostream& stream, const SceneDef& object);
	void SceneDef_Deserialize(histream& stream, SceneDef& object);
}