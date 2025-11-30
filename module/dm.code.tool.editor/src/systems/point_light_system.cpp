#include "point_light_system.h"

#include "core/core.h"
#include "core/stl/map.h"

#include "ecs/components/transform_component.h"
#include "ecs/components/color_component.h"
#include "ecs/components/point_light_component.h"

#include <glm/gtc/constants.hpp>
#include "core/utils.h"

namespace hdn
{
	struct PointLightPushConstants
	{
		vec4f32 position{};
		vec4f32 color{};
		f32 radius;
	};

	PointLightSystem::PointLightSystem(VulkanDevice* device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
	{
		Init(device, renderPass, globalSetLayout);
	}

	PointLightSystem::~PointLightSystem()
	{
		vkDestroyPipelineLayout(m_Device->get_device(), m_PipelineLayout, nullptr);
	}

	void PointLightSystem::create_pipeline_layout(VkDescriptorSetLayout globalSetLayout)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PointLightPushConstants);

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

	void PointLightSystem::create_pipeline(VkRenderPass renderPass)
	{
		assert(m_PipelineLayout);

		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::default_pipeline_config_info(pipelineConfig);
		VulkanPipeline::enable_alpha_blending(pipelineConfig);

		pipelineConfig.bindingDescriptions.clear();
		pipelineConfig.attributeDescriptions.clear();

		pipelineConfig.renderPass = renderPass; // A render pass describe the structure and format of our framebuffer objects and their attachments
		pipelineConfig.pipelineLayout = m_PipelineLayout;

		m_Pipeline = make_scope<VulkanPipeline>(m_Device, get_data_path("shaders/point_light.vert.spv"), get_data_path("shaders/point_light.frag.spv"), pipelineConfig);
	}

	void PointLightSystem::update(FrameInfo& frameInfo, GlobalUbo& ubo)
	{
		auto rotateLight = glm::rotate(mat4f32(1.0f), frameInfo.frameTime, { 0.0f, -1.0f, 0.0f });

		int lightIndex = 0;
		auto query = frameInfo.scene->World()->query<TransformComponent, ColorComponent, PointLightComponent>();
		query.each([&](flecs::entity e, TransformComponent& transformC, ColorComponent& colorC, PointLightComponent& pointLightC) {
			assert(lightIndex < MAX_LIGHTS && "Point Light exceed maximum specified");
			transformC.position = vec3f32(rotateLight * vec4f32(transformC.position, 1.0f));

			// copy light to ubo
			ubo.pointLights[lightIndex].position = vec4f32(transformC.position, 1.0f);
			ubo.pointLights[lightIndex].color = vec4f32(colorC.color, pointLightC.lightIntensity);
			lightIndex += 1;
		});
		ubo.numLights = lightIndex;
	}

	void PointLightSystem::render(FrameInfo& frameInfo)
	{
		// Sort Lights
		map<float, flecs::entity> sorted;
		auto query = frameInfo.scene->World()->query<TransformComponent, PointLightComponent>();
		query.each([&](flecs::entity e, TransformComponent& transformC, PointLightComponent& pointLightC) {
			// Calculate Distance
			auto offset = frameInfo.camera->get_position() - transformC.position;
			float distSquared = glm::dot(offset, offset);
			sorted[distSquared] = e;
		});
		m_Pipeline->bind(frameInfo.commandBuffer);

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
			flecs::entity e = it->second;

			const TransformComponent* transformC = e.get<TransformComponent>();
			const ColorComponent* colorC = e.get<ColorComponent>();
			const PointLightComponent* pointLightC = e.get<PointLightComponent>();

			PointLightPushConstants push{};
			push.position = vec4f32(transformC->position, 1.0f);
			push.color = vec4f32(colorC->color, pointLightC->lightIntensity);
			push.radius = transformC->scale.x;
			vkCmdPushConstants(frameInfo.commandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PointLightPushConstants), &push);
			vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
		}
	}
}
