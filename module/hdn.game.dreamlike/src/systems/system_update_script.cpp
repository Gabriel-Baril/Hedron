#include "system_update_script.h"
#include "ecs/components/native_script_component.h"

namespace hdn
{
	void UpdateScriptSystem::update(FrameInfo& frameInfo, flecs::world world)
	{
		auto query = world.query<NativeScriptComponent>();
		query.each([&frameInfo](flecs::entity e, NativeScriptComponent& nativeScriptC) {
			nativeScriptC.update(frameInfo.frameTime);
		});
	}
}
