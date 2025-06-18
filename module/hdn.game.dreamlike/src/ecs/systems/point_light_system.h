
#pragma once

#include "r_vk_device.h"
#include "r_vk_pipeline.h"
#include "gameobject.h"
#include "camera.h"
#include "frame_info.h"

#include "core/stl/vector.h"

namespace hdn
{

	class PointLightSystem
	{
	public:
		PointLightSystem(VulkanDevice* device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		virtual ~PointLightSystem();
		PointLightSystem(const PointLightSystem&) = delete;
		PointLightSystem& operator=(const PointLightSystem&) = delete;

		void update(FrameInfo& frameInfo, GlobalUbo& ubo);
		void render(FrameInfo& frameInfo);
	private:
		void create_pipeline_layout(VkDescriptorSetLayout globalSetLayout);
		void create_pipeline(VkRenderPass renderPass);
	private:
		VulkanDevice* m_Device;
		Scope<VulkanPipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
	};
}