#include "update_transform_system.h"

#include "hobj/scene/transform_component.h"

namespace hdn
{
	void UpdateTransformSystem::Update(FrameInfo& frameInfo)
	{
		auto query = frameInfo.ecsWorld->query<TransformComponent>();
		query.each([&](flecs::entity e, TransformComponent& transformC) {
			transformC.worldMatrix = transformC.Mat4();
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
