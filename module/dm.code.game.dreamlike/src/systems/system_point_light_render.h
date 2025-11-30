#pragma once
#include "gfx/r_vk_pipeline.h"

#include "core/core.h"
#include "core/stl/map.h"
#include "flecs/flecs.h"
#include "core/stl/vector.h"

namespace dm
{
	void system_point_light_init(VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout, VulkanDevice *device);
	void system_point_light_shutdown();

	void system_point_light_render(VkCommandBuffer commandBuffer, VkDescriptorSet globalDescriptorSet);
	void system_point_light_fetch(flecs::world world, vec3f32 cameraPos);
	void system_point_light_queue();
}
