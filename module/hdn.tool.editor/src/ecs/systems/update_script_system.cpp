#include "update_script_system.h"
#include "ecs/components/native_script_component.h"

namespace hdn
{
	void UpdateScriptSystem::update(FrameInfo& frameInfo)
	{
		auto query = frameInfo.scene->World()->query<NativeScriptComponent>();
		query.each([&](flecs::entity e, NativeScriptComponent& nativeScriptC) {
			nativeScriptC.update(frameInfo.frameTime);
		});
	}
}
