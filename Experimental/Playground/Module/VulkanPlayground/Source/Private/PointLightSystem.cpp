#include "VulkanPlayground/PointLightSystem.h"

#include "Core/Core.h"
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <map>

namespace hdn
{
	struct PointLightPushConstants
	{
		vec4f32 position{};
		vec4f32 color{};
		float32 radius;
	};

	PointLightSystem::PointLightSystem(HDNDevice* device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
		: m_Device{ device }
	{
		CreatePipelineLayout(globalSetLayout);
		CreatePipeline(renderPass);
	}

	PointLightSystem::~PointLightSystem()
	{
		vkDestroyPipelineLayout(m_Device->GetDevice(), m_PipelineLayout, nullptr);
	}

	void PointLightSystem::CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PointLightPushConstants);

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

	void PointLightSystem::CreatePipeline(VkRenderPass renderPass)
	{
		assert(m_PipelineLayout);

		PipelineConfigInfo pipelineConfig{};
		HDNPipeline::DefaultPipelineConfigInfo(pipelineConfig);
		HDNPipeline::EnableAlphaBlending(pipelineConfig);

		pipelineConfig.bindingDescriptions.clear();
		pipelineConfig.attributeDescriptions.clear();

		pipelineConfig.renderPass = renderPass; // A render pass describe the structure and format of our framebuffer objects and their attachments
		pipelineConfig.pipelineLayout = m_PipelineLayout;
		m_Pipeline = CreateScope<HDNPipeline>(m_Device, "Shaders/point_light.vert.spv", "Shaders/point_light.frag.spv", pipelineConfig);
	}

	void PointLightSystem::Update(FrameInfo& frameInfo, GlobalUbo& ubo)
	{
		auto rotateLight = glm::rotate(mat4f32(1.0f), frameInfo.frameTime, { 0.0f, -1.0f, 0.0f });

		int lightIndex = 0;
		for (auto& [key, obj] : *frameInfo.gameObjects)
		{
			if (obj.pointLight == nullptr) continue;

			assert(lightIndex < MAX_LIGHTS && "Point Light exceed maximum specified");

			obj.transform.translation = vec3f32(rotateLight * vec4f32(obj.transform.translation, 1.0f));

			// copy light to ubo
			ubo.pointLights[lightIndex].position = vec4f32(obj.transform.translation, 1.0f);
			ubo.pointLights[lightIndex].color = vec4f32(obj.color, obj.pointLight->lightIntensity);
			lightIndex += 1;
		}
		ubo.numLights = lightIndex;
	}

	void PointLightSystem::Render(FrameInfo& frameInfo)
	{
		// Sort Lights
		std::map<float, HDNGameObject::id_t> sorted;
		for (auto& [key, obj] : *frameInfo.gameObjects)
		{
			if (obj.pointLight == nullptr) continue;
			// Calculate Distance
			auto offset = frameInfo.camera->GetPosition() - obj.transform.translation;
			float distSquared = glm::dot(offset, offset);
			sorted[distSquared] = obj.GetID();
		}

		m_Pipeline->Bind(frameInfo.commandBuffer);

		vkCmdBindDescriptorSets(
			frameInfo.commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_PipelineLayout,
			0, 1,
			&frameInfo.globalDescriptorSet,
			0, nullptr
		); // Low frequency descriptor sets needs to occupy the lowest index

		// iterate through sorted lights in reverse order (back to front)
		for (auto it = sorted.rbegin(); it != sorted.rend(); it++)
		{
			auto& obj = frameInfo.gameObjects->at(it->second);

			PointLightPushConstants push{};
			push.position = vec4f32(obj.transform.translation, 1.0f);
			push.color = vec4f32(obj.color, obj.pointLight->lightIntensity);
			push.radius = obj.transform.scale.x;
			vkCmdPushConstants(frameInfo.commandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PointLightPushConstants), &push);
			vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
		}
	}
}
