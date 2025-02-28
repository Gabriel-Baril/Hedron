#pragma once

#include "core/core.h"
#include "frame_info.h"

namespace hdn
{
	class PhysicsGameObjectSystem
	{
	public:
		void Update(FrameInfo& frameInfo);
	};
}