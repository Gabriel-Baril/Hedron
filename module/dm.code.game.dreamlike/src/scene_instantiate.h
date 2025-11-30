#pragma once
#include "common/scene_db.h"

namespace hdn
{
	void entity_add_component(Entity* ent, ComponentDef* cDef);
	void entity_instantiate_prefab(Scene* scene, Entity* ent, const PrefabDef* pDef);
	void scene_instantiate(SceneDef* sceneDef);
}