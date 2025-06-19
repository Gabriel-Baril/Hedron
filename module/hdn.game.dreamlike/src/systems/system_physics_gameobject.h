#pragma once

#include "core/core.h"
#include "frame_info.h"

#include "core/application/system.h"

namespace hdn
{
	inline static constexpr const char* NAME_PHYSICS_GAMEOBJECT_SYSTEM = "PhysicsGameObjectSystem";

	class PhysicsGameObjectSystem : public ISystem
	{
	public:
		void update(FrameInfo& frameInfo, flecs::world world);
	};
}