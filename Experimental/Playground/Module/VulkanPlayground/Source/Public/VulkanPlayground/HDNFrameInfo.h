#pragma once

#include "HDNCamera.h"
#include "HDNGameObject.h"

#include "vulkan/vulkan.h"

namespace hdn
{
	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		HDNCamera* camera;
		VkDescriptorSet globalDescriptorSet;
		HDNGameObject::Map* gameObjects;
	};
}