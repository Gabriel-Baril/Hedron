#include "system_point_light_render.h"

#include "hobj/scene/transform_component.h"
#include "ecs/components/color_component.h"
#include "ecs/components/point_light_component.h"
#include "hobj/utils.h"
#include "hobj/hobj_registry.h"

namespace hdn
{
	struct PointLightPushConstants
	{
		vec4f32 position{};
		vec4f32 color{};
		f32 radius;
	};

	struct PointLightRenderGlob
	{
		map<f32, flecs::entity> sorted;
		vector<PointLightPushConstants> lights;
		Scope<VulkanPipeline> pipeline;
		VkPipelineLayout layout;
		VulkanDevice* device; // TODO: Convert to Ref<VulkanDevice>
	};

	static PointLightRenderGlob s_PointLightGlob;

	static void create_pipeline_layout(VkDescriptorSetLayout globalSetLayout)
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
		if (vkCreatePipelineLayout(s_PointLightGlob.device->get_device(), &pipelineLayoutInfo, nullptr, &s_PointLightGlob.layout) != VK_SUCCESS)
		{
			HTHROW(std::runtime_error, "Failed to create pipeline layout");
		}
	}

	static void create_pipeline(VkRenderPass renderPass)
	{
		HASSERT(s_PointLightGlob.layout, "");

		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::default_pipeline_config_info(pipelineConfig);
		VulkanPipeline::enable_alpha_blending(pipelineConfig);

		pipelineConfig.bindingDescriptions.clear();
		pipelineConfig.attributeDescriptions.clear();

		pipelineConfig.renderPass = renderPass; // A render pass describe the structure and format of our framebuffer objects and their attachments
		pipelineConfig.pipelineLayout = s_PointLightGlob.layout;

		HConfigurator* configurator = HObjectRegistry::get<HConfigurator>("config");
		s_PointLightGlob.pipeline = make_scope<VulkanPipeline>(s_PointLightGlob.device, get_data_path(configurator, "shaders/point_light.vert.spv"), get_data_path(configurator, "shaders/point_light.frag.spv"), pipelineConfig);
	}

	void system_point_light_init(VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout, VulkanDevice* device)
	{
		s_PointLightGlob.device = device;
		create_pipeline_layout(globalSetLayout);
		create_pipeline(renderPass);
	}

	void system_point_light_shutdown()
	{
		vkDestroyPipelineLayout(s_PointLightGlob.device->get_device(), s_PointLightGlob.layout, nullptr);
	}

	void system_point_light_render(VkCommandBuffer commandBuffer, VkDescriptorSet globalDescriptorSet)
	{
		s_PointLightGlob.pipeline->bind(commandBuffer);
		vkCmdBindDescriptorSets(
			commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			s_PointLightGlob.layout,
			0, 1,
			&globalDescriptorSet,
			0, nullptr
		); // Low frequency descriptor sets needs to occupy the lowest index

		// iterate through sorted lights in reverse order (back to front)
		for (auto it = s_PointLightGlob.lights.rbegin(); it != s_PointLightGlob.lights.rend(); it++)
		{
			const PointLightPushConstants& push = *it;

			vkCmdPushConstants(commandBuffer, s_PointLightGlob.layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PointLightPushConstants), &push);
			vkCmdDraw(commandBuffer, 6, 1, 0, 0);
		}
	}

	void system_point_light_fetch(flecs::world world, vec3f32 cameraPos)
	{
		s_PointLightGlob.lights.clear();
		auto query = world.query<TransformComponent, PointLightComponent>();
		query.each([&cameraPos](flecs::entity e, TransformComponent& transformC, PointLightComponent& pointLightC) {
			// Calculate Distance
			const vec3f32 offset = cameraPos - transformC.position;
			float distSquared = glm::dot(offset, offset);
			// if (distSquared < 10 * 10)
			// {
			s_PointLightGlob.sorted[distSquared] = e;
			// }
			});
	}

	void system_point_light_queue()
	{
		auto& sortedLights = s_PointLightGlob.sorted;

		// iterate through sorted lights in reverse order (back to front)
		for (auto it = sortedLights.rbegin(); it != sortedLights.rend(); it++)
		{
			flecs::entity e = it->second;

			const TransformComponent* transformC = e.get<TransformComponent>();
			const ColorComponent* colorC = e.get<ColorComponent>();
			const PointLightComponent* pointLightC = e.get<PointLightComponent>();

			PointLightPushConstants push{};
			push.position = vec4f32(transformC->position, 1.0f);
			push.color = vec4f32(colorC->color, pointLightC->lightIntensity);
			push.radius = transformC->scale.x;

			s_PointLightGlob.lights.emplace_back(push);
		}
	}
}
