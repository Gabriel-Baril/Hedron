#include "VulkanPlayground/SimpleRenderSystem.h"

#include "Core/Core.h"
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

namespace hdn
{
	struct SimplePushConstantData
	{
		mat4f32 transform{ 1.0f };
		alignas(16) vec3f32 color;
	};

	SimpleRenderSystem::SimpleRenderSystem(HDNDevice* device, VkRenderPass renderPass)
		: m_Device{device}
	{
		CreatePipelineLayout();
		CreatePipeline(renderPass);
	}

	SimpleRenderSystem::~SimpleRenderSystem()
	{
		vkDestroyPipelineLayout(m_Device->device(), m_PipelineLayout, nullptr);
	}

	void SimpleRenderSystem::CreatePipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr; // Used to pass data other than our vertex data, to our vertex and fragment shader. For example, texture and uniform buffer.
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange; // A way to push a very small amount of data to our shader
		if (vkCreatePipelineLayout(m_Device->device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
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
		m_Pipeline = std::make_unique<HDNPipeline>(m_Device, "Shaders/simple_shader.vert.spv", "Shaders/simple_shader.frag.spv", pipelineConfig);
	}

	void SimpleRenderSystem::RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<HDNGameObject>& gameObjects, const HDNCamera& camera)
	{
		m_Pipeline->Bind(commandBuffer);

		auto projectionView = camera.GetProjection() * camera.GetView();
		for (auto& obj : gameObjects)
		{
			obj.transform.rotation.y = glm::mod<float32>(obj.transform.rotation.y + 0.001f, glm::two_pi<float32>());
			obj.transform.rotation.x = glm::mod<float32>(obj.transform.rotation.x + 0.0005f, glm::two_pi<float32>());

			SimplePushConstantData push{};
			push.transform = projectionView * obj.transform.mat4(); // Will be done in the shader once we have uniform buffer
			push.color = obj.color;
			vkCmdPushConstants(commandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
			obj.model->Bind(commandBuffer);
			obj.model->Draw(commandBuffer);
		}
	}
}
