#pragma once

#include "core/core.h"
#include "frame_info.h"

namespace dm
{
	class PhysicsGameObjectSystem
	{
	public:
		void update(FrameInfo &frameInfo);
	};
}
