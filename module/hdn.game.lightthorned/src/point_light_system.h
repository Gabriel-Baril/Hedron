
#pragma once

#include "hdn_device.h"
#include "hdn_game_object.h"
#include "hdn_pipeline.h"
#include "hdn_camera.h"
#include "hdn_frame_info.h"

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

		void Update(FrameInfo& frameInfo, GlobalUbo& ubo);
		void Render(FrameInfo& frameInfo);
	private:
		void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void CreatePipeline(VkRenderPass renderPass);
	private:
		HDNDevice* m_Device;
		Scope<HDNPipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
	};
}