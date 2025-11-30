#pragma once

#include "frame_info.h"
#include "core/application/system.h"

namespace dm
{
	inline static constexpr const char *NAME_UPDATE_TRANSFORM_SYSTEM = "UpdateTransformSystem";

	class UpdateTransformSystem : public ISystem
	{
	public:
		void update(flecs::world world);
	};
}
