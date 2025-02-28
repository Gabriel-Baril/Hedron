
#pragma once

#include "r_vk_device.h"
#include "r_vk_pipeline.h"
#include "gameobject.h"
#include "frame_info.h"

#include "core/stl/vector.h"

namespace hdn
{

	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem();
		SimpleRenderSystem(VulkanDevice* device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		virtual ~SimpleRenderSystem();

		void Init(VulkanDevice* device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
		{
			m_Device = device;
			CreatePipelineLayout(globalSetLayout);
			CreatePipeline(renderPass);
		}

		void RenderGameObjects(FrameInfo& frameInfo);
	private:
		void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void CreatePipeline(VkRenderPass renderPass);
	private:
		VulkanDevice* m_Device;
		Scope<VulkanPipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
	};
}