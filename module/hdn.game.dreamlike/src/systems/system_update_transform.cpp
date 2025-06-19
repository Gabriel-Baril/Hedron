#include "system_update_transform.h"

#include "hobj/scene/transform_component.h"

namespace hdn
{
	void UpdateTransformSystem::update(flecs::world world)
	{
		auto query = world.query<TransformComponent>();
		query.each([&](flecs::entity e, TransformComponent& transformC) {
			transformC.worldMatrix = transformC.to_mat();
			if (e.parent())
			{
				auto parent = e.parent();
				if (parent.has<TransformComponent>()) {
					transformC.worldMatrix = parent.get<TransformComponent>()->worldMatrix * transformC.worldMatrix;
				}
			}
		});
	}
}
