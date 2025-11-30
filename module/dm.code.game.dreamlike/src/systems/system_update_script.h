#pragma once

#include "frame_info.h"
#include "core/application/system.h"

namespace dm
{
	inline static constexpr const char *NAME_UPDATE_SCRIPT_SYSTEM = "UpdateScriptSystem";

	class UpdateScriptSystem : public ISystem
	{
	public:
		void update(FrameInfo &frameInfo, flecs::world world);
	};
}
