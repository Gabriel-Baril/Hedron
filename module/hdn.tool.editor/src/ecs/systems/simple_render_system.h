
#pragma once

#include "r_vk_device.h"
#include "r_vk_pipeline.h"
#include "gameobject.h"
#include "camera.h"
#include "frame_info.h"

#include "core/stl/vector.h"

namespace hdn
{

	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(VulkanDevice* device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		virtual ~SimpleRenderSystem();
		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void RenderGameObjects(FrameInfo& frameInfo);
	private:
		void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void CreatePipeline(VkRenderPass renderPass);
	private:
		VulkanDevice* m_Device;
		Scope<VulkanPipeline> m_Pipeline; // TODO: Change to Scope<>
		VkPipelineLayout m_PipelineLayout;
	};
}