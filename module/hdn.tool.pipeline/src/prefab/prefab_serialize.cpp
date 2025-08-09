#include "prefab.h"

#include "prefab_component_uuid.h"
#include "prefab_component_transform.h"
#include "prefab_component_model.h"
#include "prefab_component_hierarchy.h"

namespace hdn
{
	void prefab_set_world(SPrefabDef& prefab, flecs::world world)
	{
		prefab.world = world;
	}

	void prefab_serialize(hostream& stream, const SPrefabDef& prefab)
	{
		u32 entityCount = 0;
		const u64 entityCountMarker = stream.marker();
		stream << entityCount;
		prefab.world.each([&entityCount, &prefab, &stream](flecs::entity e, PrefabUUIDComponent& uuidC) {
			entityCount++;
			u32 componentCount = 0;
			const u64 componentCountMarker = stream.marker();
			stream << componentCount;
			e.each([&prefab, &stream, &componentCount, &e](flecs::id comp_id)
				{
					if (comp_id == prefab.world.id<PrefabUUIDComponent>()) {
						componentCount++;
						const PrefabUUIDComponent* c = e.get<PrefabUUIDComponent>();
						stream << underlying(PrefabComponentType::UUID_COMPONENT);
						stream << c->uuid;
					}
					else if (comp_id == prefab.world.id<PrefabTransformComponent>())
					{
						componentCount++;
						const PrefabTransformComponent* c = e.get<PrefabTransformComponent>();
						stream << underlying(PrefabComponentType::TRANSFORM_COMPONENT);
						stream << c->position;
						stream << c->rotation;
						stream << c->scale;
					}
					else if (comp_id == prefab.world.id<PrefabModelComponent>()) {
						componentCount++;
						const PrefabModelComponent* c = e.get<PrefabModelComponent>();
						stream << underlying(PrefabComponentType::MODEL_COMPONENT);
						stream << c->dummy;
					}
					else if (comp_id == prefab.world.id<PrefabHierarchyComponent>()) {
						componentCount++;
						const PrefabHierarchyComponent* c = e.get<PrefabHierarchyComponent>();
						stream << underlying(PrefabComponentType::HIERARCHY_COMPONENT);
						stream << c->dummy;
					}
				});
			stream.write_at((const void*)&componentCount, sizeof(componentCount), componentCountMarker); // Patch component count
			});
		stream.write_at((const void*)&entityCount, sizeof(entityCount), entityCountMarker); // Patch entity count
	}

	static void prefab_deserialize_component(histream& stream, SPrefabDef& prefab, flecs::entity ent, PrefabComponentType type)
	{
		switch (type)
		{
		case PrefabComponentType::UUID_COMPONENT:
		{
			PrefabUUIDComponent uuidC;
			stream >> uuidC.uuid;
			ent.set(uuidC);
			break;
		}
		case PrefabComponentType::TRANSFORM_COMPONENT:
		{
			PrefabTransformComponent transformC;
			stream >> transformC.position;
			stream >> transformC.rotation;
			stream >> transformC.scale;
			ent.set(transformC);
			break;
		}
		case PrefabComponentType::MODEL_COMPONENT:
		{
			PrefabModelComponent modelC;
			stream >> modelC.dummy;
			ent.set(modelC);
			break;
		}
		case PrefabComponentType::HIERARCHY_COMPONENT:
		{
			PrefabModelComponent hierarchyC;
			stream >> hierarchyC.dummy;
			ent.set(hierarchyC);
			break;
		}
		default:
			break;
		}
	}

	void prefab_deserialize(histream& stream, SPrefabDef& prefab)
	{
		u32 entityCount = 0;
		stream >> entityCount;
		for (u32 i = 0; i < entityCount; i++)
		{
			flecs::entity ent = prefab.world.entity();
			u32 componentCount = 0;
			stream >> componentCount;
			for (u32 j = 0; j < componentCount; j++)
			{
				PrefabComponentType type;
				stream >> type;
				prefab_deserialize_component(stream, prefab, ent, type);
			}
		}
	}
}