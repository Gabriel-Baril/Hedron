
#pragma once

#include "gfx/r_vk_device.h"
#include "gfx/r_vk_pipeline.h"
#include "gameobject.h"
#include "camera.h"
#include "frame_info.h"

#include "core/stl/vector.h"
#include "core/application/system.h"

namespace hdn
{
	inline static constexpr const char* NAME_POINT_LIGHT_SYSTEM = "PointLightSystem";

	class PointLightSystem : public ISystem
	{
	public:
		PointLightSystem(VulkanDevice* device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		virtual ~PointLightSystem();
		PointLightSystem(const PointLightSystem&) = delete;
		PointLightSystem& operator=(const PointLightSystem&) = delete;

		void update(FrameInfo& frameInfo, flecs::world world);
		void render(FrameInfo& frameInfo, flecs::world world);
	private:
		void create_pipeline_layout(VkDescriptorSetLayout globalSetLayout);
		void create_pipeline(VkRenderPass renderPass);
	private:
		VulkanDevice* m_Device;
		Scope<VulkanPipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
	};
}