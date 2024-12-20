#include "SimpleRenderSystem.h"

#include "core/Core.h"
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

namespace hdn
{
	struct SimplePushConstantData
	{
		mat4f32 modelMatrix{ 1.0f };
		mat4f32 normalMatrix{ 1.0f };
	};

	SimpleRenderSystem::SimpleRenderSystem(HDNDevice* device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
		: m_Device{ device }
	{
		CreatePipelineLayout(globalSetLayout);
		CreatePipeline(renderPass);
	}

	SimpleRenderSystem::~SimpleRenderSystem()
	{
		vkDestroyPipelineLayout(m_Device->GetDevice(), m_PipelineLayout, nullptr);
	}

	void SimpleRenderSystem::CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data(); // Used to pass data other than our vertex data, to our vertex and fragment shader. For example, texture and uniform buffer.
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange; // A way to push a very small amount of data to our shader
		if (vkCreatePipelineLayout(m_Device->GetDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout");
		}
	}

	void SimpleRenderSystem::CreatePipeline(VkRenderPass renderPass)
	{
		assert(m_PipelineLayout);

		PipelineConfigInfo pipelineConfig{};
		HDNPipeline::DefaultPipelineConfigInfo(pipelineConfig);

		pipelineConfig.renderPass = renderPass; // A render pass describe the structure and format of our framebuffer objects and their attachments
		pipelineConfig.pipelineLayout = m_PipelineLayout;
		m_Pipeline = CreateScope<HDNPipeline>(m_Device, "Shaders/simple_shader.vert.spv", "Shaders/simple_shader.frag.spv", pipelineConfig);
	}

	void SimpleRenderSystem::RenderGameObjects(FrameInfo& frameInfo)
	{
		m_Pipeline->Bind(frameInfo.commandBuffer);

		vkCmdBindDescriptorSets(
			frameInfo.commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_PipelineLayout,
			0, 1,
			&frameInfo.globalDescriptorSet,
			0, nullptr
		); // Low frequency descriptor sets needs to occupy the lowest index

		for (auto& [key, obj] : *frameInfo.gameObjects)
		{
			if (obj.model == nullptr) continue; // TODO: We should query an ECS instead of manually creating such filter

			SimplePushConstantData push{};
			push.modelMatrix = obj.transform.Mat4(); // Will be done in the shader once we have uniform buffer
			push.normalMatrix = obj.transform.NormalMatrix();
			vkCmdPushConstants(frameInfo.commandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
			obj.model->Bind(frameInfo.commandBuffer);
			obj.model->Draw(frameInfo.commandBuffer);
		}
	}
}