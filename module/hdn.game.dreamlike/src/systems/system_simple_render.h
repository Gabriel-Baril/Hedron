
#pragma once

#include "r_vk_device.h"
#include "r_vk_pipeline.h"
#include "gameobject.h"
#include "camera.h"
#include "frame_info.h"

#include "core/stl/vector.h"
#include "core/application/system.h"

namespace hdn
{
	inline static constexpr const char* NAME_SIMPLE_RENDER_SYSTEM = "SimpleRenderSystem";

	class SimpleRenderSystem : public ISystem
	{
	public:
		SimpleRenderSystem(VulkanDevice* device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		virtual ~SimpleRenderSystem();
		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void render(FrameInfo& frameInfo, flecs::world world);
	private:
		void create_pipeline_layout(VkDescriptorSetLayout globalSetLayout);
		void create_pipeline(VkRenderPass renderPass);
	private:
		VulkanDevice* m_Device;
		Scope<VulkanPipeline> m_Pipeline; // TODO: Change to Scope<>
		VkPipelineLayout m_PipelineLayout;
	};
}