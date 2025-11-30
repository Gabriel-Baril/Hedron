#include "system_simple_render.h"

#include "core/core.h"
#include <glm/gtc/constants.hpp>

#include "hobj/scene/transform_component.h"
#include "ecs/components/model_component.h"
#include "hobj/utils.h"
#include "hobj/hobj_registry.h"

namespace hdn
{
	struct SimplePushConstantData
	{
		mat4f32 modelMatrix{ 1.0f };
		mat4f32 normalMatrix{ 1.0f };
	};

	SimpleRenderSystem::SimpleRenderSystem(VulkanDevice* device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
		: m_Device{ device }
	{
		create_pipeline_layout(globalSetLayout);
		create_pipeline(renderPass);
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

		vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<u32>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data(); // Used to pass data other than our vertex data, to our vertex and fragment shader. For example, texture and uniform buffer.
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange; // A way to push a very small amount of data to our shader
		if (vkCreatePipelineLayout(m_Device->get_device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
		{
			HDN_CORE_THROW(std::runtime_error, "Failed to create pipeline layout");
		}
	}

	void SimpleRenderSystem::create_pipeline(VkRenderPass renderPass)
	{
		assert(m_PipelineLayout);

		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::default_pipeline_config_info(pipelineConfig);

		pipelineConfig.renderPass = renderPass; // A render pass describe the structure and format of our framebuffer objects and their attachments
		pipelineConfig.pipelineLayout = m_PipelineLayout;

		HConfigurator* configurator = HObjectRegistry::get<HConfigurator>("config");
		m_Pipeline = make_scope<VulkanPipeline>(m_Device, get_data_path(configurator, "shaders/simple_shader.vert.spv"), get_data_path(configurator, "shaders/simple_shader.frag.spv"), pipelineConfig);
	}

	void SimpleRenderSystem::render(FrameInfo& frameInfo, flecs::world world)
	{
		m_Pipeline->bind(frameInfo.commandBuffer);

		vkCmdBindDescriptorSets(
			frameInfo.commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_PipelineLayout,
			0, 1,
			&frameInfo.globalDescriptorSet,
			0, nullptr
		); // Low frequency descriptor sets needs to occupy the lowest index

		auto query = world.query<TransformComponent, ModelComponent>();
		query.each([&](flecs::entity e, TransformComponent& transformC, ModelComponent& modelC) {

			SimplePushConstantData push{};
			push.modelMatrix = transformC.to_mat(); // Will be done in the shader once we have uniform buffer
			push.normalMatrix = transformC.normal_matrix();
			vkCmdPushConstants(frameInfo.commandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
			modelC.model->bind(frameInfo.commandBuffer);
			modelC.model->draw(frameInfo.commandBuffer);
		});
	}
}