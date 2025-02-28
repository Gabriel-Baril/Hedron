#pragma once

#include "core/stl/vector.h"

#include "r_vk_device.h"

namespace hdn
{
	struct PipelineConfigInfo
	{
		PipelineConfigInfo() = default;
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		vector<VkVertexInputBindingDescription> bindingDescriptions{};
		vector<VkVertexInputAttributeDescription> attributeDescriptions{};

		VkPipelineViewportStateCreateInfo viewportInfo{};
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
		VkPipelineMultisampleStateCreateInfo multisampleInfo{};
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
		vector<VkDynamicState> dynamicStateEnables{};
		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};

		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class VulkanPipeline
	{
	public:
		VulkanPipeline(
			VulkanDevice* device, 
			const string& vertFilepath, 
			const string& fragFilepath, 
			const PipelineConfigInfo& configInfo);
		virtual ~VulkanPipeline();
		VulkanPipeline() = default;
		VulkanPipeline(const VulkanPipeline&) = delete;
		VulkanPipeline& operator=(const VulkanPipeline&) = delete;
		VulkanPipeline(VulkanPipeline&&) = delete;
		VulkanPipeline& operator=(VulkanPipeline&&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
		static void EnableAlphaBlending(PipelineConfigInfo& configInfo);
	private:
		static vector<char> ReadFile(const string& filepath);
		void CreateGraphicsPipeline(
			const string& vertFilepath,
			const string& fragFilepath,
			const PipelineConfigInfo& configInfo);
		void CreateShaderModule(const vector<char>& code, VkShaderModule* module);
	private:
		VulkanDevice* m_Device;
		VkPipeline m_GraphicsPipeline;
		VkShaderModule m_VertShaderModule;
		VkShaderModule m_FragShaderModule;
	};
}