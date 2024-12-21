#include "HDNPipeline.h"

#include "core/Core.h"
#include <fstream>
#include <stdexcept>
#include <cassert> // TODO: Use HDN_ASSERT

#include "HDNModel.h"

namespace hdn
{
	HDNPipeline::HDNPipeline(HDNDevice* device, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo)
		: m_Device{ device }
	{
		CreateGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
	}

	HDNPipeline::~HDNPipeline()
	{
		vkDestroyShaderModule(m_Device->GetDevice(), m_VertShaderModule, nullptr);
		vkDestroyShaderModule(m_Device->GetDevice(), m_FragShaderModule, nullptr);
		vkDestroyPipeline(m_Device->GetDevice(), m_GraphicsPipeline, nullptr);
	}

	void HDNPipeline::Bind(VkCommandBuffer commandBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline); // Signal that we want to bind a graphics pipeline
	}

	void HDNPipeline::DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo)
	{
		configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; // Define how our vertex are related topologically
		configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configInfo.viewportInfo.viewportCount = 1;
		configInfo.viewportInfo.pViewports = nullptr;
		configInfo.viewportInfo.scissorCount = 1;
		configInfo.viewportInfo.pScissors = nullptr;

		// Breaks up our geometry into fragment
		configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo.rasterizationInfo.depthClampEnable = VK_FALSE; // Force the Z of gl_Position to be between 0 and 1 (require a gpu feature to be activated to work)
		configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE; // 
		configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		configInfo.rasterizationInfo.lineWidth = 1.0f;
		configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE; // Discard triangle based on their apparent facing (winding order)
		configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE; 
		configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE; // Used to alter depth values
		configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional
		configInfo.rasterizationInfo.depthBiasClamp = 0.0f; // Optional
		configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f; // Optional

		// Relates on how the rasterized handle edges, without multi sampling a fragment is either considered completely in or completely out of a triangle. MSAA
		configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
		configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		configInfo.multisampleInfo.minSampleShading = 1.0f; // Optional
		configInfo.multisampleInfo.pSampleMask = nullptr; // Optional
		configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
		configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE; // Optional

		// Color blending controls how we combine color in our framebuffer. If two triangle overlap with each other then our fragment shader will return multiple color for our pixel
		configInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		configInfo.colorBlendAttachment.blendEnable = VK_FALSE; // Do we want to mix the current output with the color value already in the framebuffer if any
		configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
		configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

		configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
		configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
		configInfo.colorBlendInfo.attachmentCount = 1;
		configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
		configInfo.colorBlendInfo.blendConstants[0] = 0.0f; // Optional
		configInfo.colorBlendInfo.blendConstants[1] = 0.0f; // Optional
		configInfo.colorBlendInfo.blendConstants[2] = 0.0f; // Optional
		configInfo.colorBlendInfo.blendConstants[3] = 0.0f; // Optional

		// Depth buffer is an additional attachment to our framebuffer that store the depth of each pixel 
		configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
		configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
		configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		configInfo.depthStencilInfo.minDepthBounds = 0.0f; // Optional
		configInfo.depthStencilInfo.maxDepthBounds = 1.0f; // Optional
		configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
		configInfo.depthStencilInfo.front = {}; // Optional
		configInfo.depthStencilInfo.back = {}; // Optional

		configInfo.dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.data();
		configInfo.dynamicStateInfo.dynamicStateCount = static_cast<u32>(configInfo.dynamicStateEnables.size());
		configInfo.dynamicStateInfo.flags = 0;

		configInfo.bindingDescriptions = HDNModel::Vertex::GetBindingDescriptions();
		configInfo.attributeDescriptions = HDNModel::Vertex::GetAttributeDescriptions();
	}

	void HDNPipeline::EnableAlphaBlending(PipelineConfigInfo& configInfo)
	{
		configInfo.colorBlendAttachment.blendEnable = VK_TRUE; // Do we want to mix the current output with the color value already in the framebuffer if any

		// Color blending controls how we combine color in our framebuffer. If two triangle overlap with each other then our fragment shader will return multiple color for our pixel
		configInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	}

	std::vector<char> HDNPipeline::ReadFile(const std::string& filepath)
	{
		std::ifstream file(filepath, std::ios::ate | std::ios::binary); // std::ios::ate -> When the file open we seek to the end immediately
		if (!file.is_open())
		{
			throw std::runtime_error("Failed to open file: " + filepath);
		}
		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();
		return buffer;
	}

	void HDNPipeline::CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo)
	{
		assert(configInfo.pipelineLayout != VK_NULL_HANDLE);
		assert(configInfo.renderPass != VK_NULL_HANDLE);

		const auto vertCode = ReadFile(vertFilepath);
		const auto fragCode = ReadFile(fragFilepath);

		CreateShaderModule(vertCode, &m_VertShaderModule);
		CreateShaderModule(fragCode, &m_FragShaderModule);

		VkPipelineShaderStageCreateInfo shaderStages[2];
		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = m_VertShaderModule;
		shaderStages[0].pName = "main"; // Name of our entry function in our vertex shader
		shaderStages[0].flags = 0;
		shaderStages[0].pNext = nullptr;
		shaderStages[0].pSpecializationInfo = nullptr;

		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = m_FragShaderModule;
		shaderStages[1].pName = "main"; // Name of our entry function in our vertex shader
		shaderStages[1].flags = 0;
		shaderStages[1].pNext = nullptr;
		shaderStages[1].pSpecializationInfo = nullptr;

		// Controls how we interpret our vertex buffer data (which the initial info in our graphics pipeline)
		auto& attributeDescriptions = configInfo.attributeDescriptions;
		auto& bindingDescriptions = configInfo.bindingDescriptions;
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<u32>(attributeDescriptions.size());
		vertexInputInfo.vertexBindingDescriptionCount = static_cast<u32>(bindingDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
		vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages; // Programmable Stages
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
		pipelineInfo.pViewportState = &configInfo.viewportInfo;
		pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
		pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
		pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
		pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
		pipelineInfo.pDynamicState = &configInfo.dynamicStateInfo;

		pipelineInfo.layout = configInfo.pipelineLayout;
		pipelineInfo.renderPass = configInfo.renderPass;
		pipelineInfo.subpass = configInfo.subpass;

		// Can be used when trying to optimize performance
		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines(m_Device->GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create graphics pipeline");
		}
	}

	void HDNPipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule* module)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const u32*>(code.data()); // Since std::vector take into account the worst case alignment scenario, thisd reinterpret_cast is valid. It wouldn't be true for a c-style array
		if (vkCreateShaderModule(m_Device->GetDevice(), &createInfo, nullptr, module) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create shader module");
		}
	}
}
