
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

	class PointLightSystem
	{
	public:
		PointLightSystem(HDNDevice* device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		virtual ~PointLightSystem();
		PointLightSystem(const PointLightSystem&) = delete;
		PointLightSystem& operator=(const PointLightSystem&) = delete;

		void Render(FrameInfo& frameInfo);
	private:
		void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void CreatePipeline(VkRenderPass renderPass);
	private:
		HDNDevice* m_Device;
		std::unique_ptr<HDNPipeline> m_Pipeline; // TODO: Change to Scope<>
		VkPipelineLayout m_PipelineLayout;
	};
}