#include "update_transform_system.h"

#include "ecs/components/transform_component.h"

namespace hdn
{
	void UpdateTransformSystem::update(FrameInfo& frameInfo)
	{
		auto query = frameInfo.scene->World()->query<TransformComponent>();
		query.each([&](flecs::entity e, TransformComponent& transformC) {
			// if (!transformC.Changed())
			// {
			// 	return;
			// }

			transformC.worldMatrix = transformC.to_mat();
			if (e.parent())
			{
				auto parent = e.parent();
				if (parent.has<TransformComponent>()) {
					transformC.worldMatrix = parent.get<TransformComponent>()->to_mat() * transformC.worldMatrix;
				}
			}

			transformC.clear_change();
		});
	}
}
