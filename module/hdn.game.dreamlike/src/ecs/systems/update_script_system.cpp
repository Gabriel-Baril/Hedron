#include "update_script_system.h"
#include "ecs/components/native_script_component.h"

namespace hdn
{
	void UpdateScriptSystem::Update(FrameInfo& frameInfo)
	{
		auto query = frameInfo.ecsWorld->query<NativeScriptComponent>();
		query.each([&](flecs::entity e, NativeScriptComponent& nativeScriptC) {
			nativeScriptC.Update(frameInfo.frameTime);
		});
	}
}
