
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

		void Update(FrameInfo& frameInfo, GlobalUbo& ubo);
		void Render(FrameInfo& frameInfo);
	private:
		void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void CreatePipeline(VkRenderPass renderPass);
	private:
		VulkanDevice* m_Device;
		Scope<VulkanPipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
	};
}