#include "simple_render_system.h"

#include "core/core.h"
#include <glm/gtc/constants.hpp>

#include "ecs/components/transform_component.h"
#include "ecs/components/model_component.h"
#include "core/utils.h"

namespace dm
{
	struct SimplePushConstantData
	{
		mat4f32 modelMatrix{1.0f};
		mat4f32 normalMatrix{1.0f};
	};

	SimpleRenderSystem::SimpleRenderSystem()
	{
	}

	SimpleRenderSystem::SimpleRenderSystem(VulkanDevice *device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
	{
		Init(device, renderPass, globalSetLayout);
	}

	SimpleRenderSystem::~SimpleRenderSystem()
	{
		vkDestroyPipelineLayout(m_Device->get_device(), m_PipelineLayout, nullptr);
	}

	void SimpleRenderSystem::create_pipeline_layout(VkDescriptorSetLayout globalSetLayout)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<u32>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data(); // Used to pass data other than our vertex data, to our vertex and fragment shader. For example, texture and uniform buffer.
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange; // A way to push a very small amount of data to our shader
		if (vkCreatePipelineLayout(m_Device->get_device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
		{
			DM_CORE_THROW(std::runtime_error, "Failed to create pipeline layout");
		}
	}

	void SimpleRenderSystem::create_pipeline(VkRenderPass renderPass)
	{
		assert(m_PipelineLayout);

		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::default_pipeline_config_info(pipelineConfig);

		pipelineConfig.renderPass = renderPass; // A render pass describe the structure and format of our framebuffer objects and their attachments
		pipelineConfig.pipelineLayout = m_PipelineLayout;

		m_Pipeline = make_scope<VulkanPipeline>(m_Device, get_data_path("shaders/simple_shader.vert.spv"), get_data_path("shaders/simple_shader.frag.spv"), pipelineConfig);
	}

	void SimpleRenderSystem::render(FrameInfo &frameInfo)
	{
		m_Pipeline->bind(frameInfo.commandBuffer);

		vkCmdBindDescriptorSets(
				frameInfo.commandBuffer,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				m_PipelineLayout,
				0, 1,
				&frameInfo.globalDescriptorSet,
				0, nullptr); // Low frequency descriptor sets needs to occupy the lowest index

		auto query = frameInfo.scene->World()->query<TransformComponent, ModelComponent>();
		query.each([&](flecs::entity e, TransformComponent &transformC, ModelComponent &modelC)
							 {

			SimplePushConstantData push{};
			push.modelMatrix = transformC.to_mat(); // Will be done in the shader once we have uniform buffer
			push.normalMatrix = transformC.normal_matrix();
			vkCmdPushConstants(frameInfo.commandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
			modelC.model->bind(frameInfo.commandBuffer);
			modelC.model->draw(frameInfo.commandBuffer); });
	}
}
