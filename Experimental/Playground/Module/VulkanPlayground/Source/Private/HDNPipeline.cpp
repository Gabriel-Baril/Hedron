#include "VulkanPlayground/HDNPipeline.h"

#include "Core/Core.h"
#include <fstream>
#include <stdexcept>

namespace hdn
{
	HDNPipeline::HDNPipeline(HDNDevice* device, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo)
		: m_Device{ device }
	{
		CreateGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
	}

	PipelineConfigInfo HDNPipeline::DefaultPipelineConfigInfo(uint32 width, uint32 height)
	{
		PipelineConfigInfo info{};
		return info;
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
		const auto vertCode = ReadFile(vertFilepath);
		const auto fragCode = ReadFile(fragFilepath);

		HDN_CORE_INFO("Vertex Code Side: {0}", vertCode.size());
		HDN_CORE_INFO("Fragment Code Side: {0}", fragCode.size());
	}

	void HDNPipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule* module)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32*>(code.data()); // Since std::vector take into account the worst case alignment scenario, thisd reinterpret_cast is valid. It wouldn't be true for a c-style array

		if (vkCreateShaderModule(m_Device->device(), &createInfo, nullptr, module) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create shader module");
		}

	}
}
