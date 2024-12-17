#pragma once

#include "HDNCamera.h"

#include "vulkan/vulkan.h"

namespace hdn
{
	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		HDNCamera* camera;
	};
}