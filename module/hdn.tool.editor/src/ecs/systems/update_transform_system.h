#pragma once

#include "frame_info.h"

namespace hdn
{
	class UpdateTransformSystem
	{
	public:
		void update(FrameInfo& frameInfo);
	};
}