
#pragma once

#include "gfx/r_vk_device.h"
#include "gfx/r_vk_pipeline.h"
#include "frame_info.h"

#include "core/stl/vector.h"

namespace dm
{

	class PointLightSystem
	{
	public:
		PointLightSystem() = default;
		PointLightSystem(VulkanDevice *device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		virtual ~PointLightSystem();

		void Init(VulkanDevice *device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
		{
			m_Device = device;
			create_pipeline_layout(globalSetLayout);
			create_pipeline(renderPass);
		}

		void update(FrameInfo &frameInfo, GlobalUbo &ubo);
		void render(FrameInfo &frameInfo);

	private:
		void create_pipeline_layout(VkDescriptorSetLayout globalSetLayout);
		void create_pipeline(VkRenderPass renderPass);

	private:
		VulkanDevice *m_Device;
		Scope<VulkanPipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
	};
}
