#pragma once

#include <string>
#include <vector>

#include "HDNDevice.h"

namespace hdn
{
	struct PipelineConfigInfo
	{

	};

	class HDNPipeline
	{
	public:
		HDNPipeline(
			HDNDevice* device, 
			const std::string& vertFilepath, 
			const std::string& fragFilepath, 
			const PipelineConfigInfo& configInfo);
		virtual ~HDNPipeline() = default;
		HDNPipeline(const HDNPipeline&) = delete;
		void operator=(const HDNPipeline&) = delete;
		static PipelineConfigInfo DefaultPipelineConfigInfo(uint32 width, uint32 height);
	private:
		static std::vector<char> ReadFile(const std::string& filepath);
		void CreateGraphicsPipeline(
			const std::string& vertFilepath,
			const std::string& fragFilepath,
			const PipelineConfigInfo& configInfo);
		void CreateShaderModule(const std::vector<char>& code, VkShaderModule* module);
	private:
		HDNDevice* m_Device;
		VkPipeline m_GraphicsPipeline;
		VkShaderModule m_VertShaderModule;
		VkShaderModule m_FragShaderModule;
	};
}