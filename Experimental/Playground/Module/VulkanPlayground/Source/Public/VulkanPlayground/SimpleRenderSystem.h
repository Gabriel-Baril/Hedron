
#pragma once

#include "HDNDevice.h"
#include "HDNGameObject.h"
#include "HDNPipeline.h"

#include <memory>
#include <vector>

namespace hdn
{

	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(HDNDevice* device, VkRenderPass renderPass);
		virtual ~SimpleRenderSystem();
		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<HDNGameObject>& gameObjects);
	private:
		void CreatePipelineLayout();
		void CreatePipeline(VkRenderPass renderPass);
	private:
		HDNDevice* m_Device;
		std::unique_ptr<HDNPipeline> m_Pipeline; // TODO: Change to Scope<>
		VkPipelineLayout m_PipelineLayout;
	};
}