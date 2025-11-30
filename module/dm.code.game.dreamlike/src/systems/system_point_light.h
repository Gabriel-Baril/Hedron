
#pragma once

#include "gfx/r_vk_device.h"
#include "gfx/r_vk_pipeline.h"
#include "gameobject.h"
#include "camera.h"
#include "frame_info.h"

#include "core/stl/vector.h"
#include "core/application/system.h"

namespace dm
{
	inline static constexpr const char *NAME_POINT_LIGHT_SYSTEM = "PointLightSystem";

	// PointLightSystem offers a stable API for system for update
	class PointLightSystem : public ISystem
	{
	public:
		PointLightSystem(VulkanDevice *device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		virtual ~PointLightSystem();
		PointLightSystem(const PointLightSystem &) = delete;
		PointLightSystem &operator=(const PointLightSystem &) = delete;

		void update(FrameInfo &frameInfo, flecs::world world);
		void render(FrameInfo &frameInfo, flecs::world world);
	};
}
