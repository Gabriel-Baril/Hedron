#include "scene_instantiate.h"

namespace dm
{
	void entity_add_component_transform(Entity *ent, ComponentDef *cDef)
	{
		const TransformComponent *transform = static_cast<const TransformComponent *>(cDef->data);
		ent->add(*transform);
	}

	void entity_add_component_model(Entity *ent, ComponentDef *cDef)
	{
		const ModelComponent *modelC = static_cast<const ModelComponent *>(cDef->data);
		ent->add(*modelC);
	}

	void entity_add_component(Entity *ent, ComponentDef *cDef)
	{
		using ComponentAddCallback = void (*)(Entity *, ComponentDef *);
		static ComponentAddCallback callbacks[underlying(ComponentType::SIZE)] = {
				entity_add_component_transform,
				entity_add_component_model};

		const int componentTypeIndex = underlying(cDef->type);
		ComponentAddCallback callback = callbacks[componentTypeIndex];
		if (callback)
		{
			callback(ent, cDef);
		}
	}

	void entity_instantiate_prefab(Scene *scene, Entity *ent, const PrefabDef *pDef)
	{
		DM_CORE_ASSERT(ent);
		for (int i = 0; i < pDef->componentCount; i++)
		{
			const ComponentDef *component = pDef->components[i];
			entity_add_component(ent, component)
		}

		for (int i = 0; i < pDef->prefabCount; i++)
		{
			const PrefabDef *childPrefab = pDef->childPrefabs[i];
			DM_CORE_ASSERT(childPrefab);

			Entity *childEnt = nullptr;
			scene_create_entity(scene, childEnt);
			DM_CORE_ASSERT(ent);
			entity_set_parent(childEnt, ent);
			entity_instantiate_prefab(scene, childEnt, childPrefab);
		}
	}

	void scene_instantiate(SceneDef *sceneDef)
	{
		Scene scene;
		for (int i = 0; i < sceneDef->entityCount; i++)
		{
			DM_CORE_ASSERT(sceneDef->entities[i]);
			const EntityDef &eDef = sceneDef->entities[i];
			Entity *ent = nullptr;
			scene_create_entity(scene, ent);

			DM_CORE_ASSERT(ent);
			DM_CORE_ASSERT(eDef - < prefab);
			DM_CORE_ASSERT(eDef - < prefab->parent == nullptr); // Make sure the prefab is the one from a top level entity
			entity_instantiate_prefab(&scene, ent, eDef - < prefab);
		}
	}

}
