#pragma once

#include "core/core.h"
#include "../hdn_frame_info.h"

namespace hdn
{
	class PhysicsGameObjectSystem
	{
	public:
		void Update(FrameInfo& frameInfo);
	};
}