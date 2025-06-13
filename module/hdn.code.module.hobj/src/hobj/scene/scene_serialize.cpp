#include "scene.h"
#include "transform_component.h"

namespace hdn
{
	enum class SceneComponentType
	{
		TRANSFORM_COMPONENT = 0,
		COUNT
	};

	void scene_set_world(Scene& scene, flecs::world world)
	{
		scene.world = world;
	}

	void scene_serialize(hostream& stream, const Scene& scene)
	{
		u32 entityCount = 0;
		const u64 entityCountMarker = stream.marker();
		stream << entityCount;
		scene.world.each([&entityCount, &scene, &stream](flecs::entity e, TransformComponent& transformC) {
			entityCount++;
			u32 componentCount = 0;
			const u64 componentCountMarker = stream.marker();
			stream << componentCount;
			e.each([&scene, &stream, &componentCount, &e](flecs::id comp_id)
			{
				componentCount++;
				if (comp_id == scene.world.id<TransformComponent>()) {
					const TransformComponent* c = e.get<TransformComponent>();
					stream << underlying(SceneComponentType::TRANSFORM_COMPONENT);
					stream << c->position;
					stream << c->rotation;
					stream << c->scale;
				}
			});
			stream.write_at((const void*)&componentCount, sizeof(componentCount), componentCountMarker); // Patch component count
		});
		stream.write_at((const void*)&entityCount, sizeof(entityCount), entityCountMarker); // Patch entity count
	}

	static void scene_deserialize_component(histream& stream, Scene& scene, flecs::entity ent, SceneComponentType type)
	{
		switch (type)
		{
		case SceneComponentType::TRANSFORM_COMPONENT:
		{
			TransformComponent transform;
			stream >> transform.position;
			stream >> transform.rotation;
			stream >> transform.scale;
			ent.set(transform);
			break;
		}
		default:
			break;
		}
	}

	void scene_deserialize(histream& stream, Scene& scene)
	{
		u32 entityCount = 0;
		stream >> entityCount;
		for (u32 i = 0; i < entityCount; i++)
		{
			flecs::entity ent = scene.world.entity();
			u32 componentCount = 0;
			stream >> componentCount;
			for (u32 j = 0; j < componentCount; j++)
			{
				SceneComponentType type;
				stream >> type;
				scene_deserialize_component(stream, scene, ent, type);
			}
		}
	}
}
