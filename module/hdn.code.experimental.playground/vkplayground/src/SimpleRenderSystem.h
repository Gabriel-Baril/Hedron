
#pragma once

#include "HDNDevice.h"
#include "HDNGameObject.h"
#include "HDNPipeline.h"
#include "HDNCamera.h"
#include "HDNFrameInfo.h"

#include <memory>
#include <vector>

namespace hdn
{

	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(HDNDevice* device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		virtual ~SimpleRenderSystem();
		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void RenderGameObjects(FrameInfo& frameInfo);
	private:
		void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void CreatePipeline(VkRenderPass renderPass);
	private:
		HDNDevice* m_Device;
		Scope<HDNPipeline> m_Pipeline; // TODO: Change to Scope<>
		VkPipelineLayout m_PipelineLayout;
	};
}