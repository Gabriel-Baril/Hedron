
#pragma once

#include "HDNWindow.h"
#include "HDNPipeline.h"
#include "HDNDevice.h"

namespace hdn
{
	class FirstApp
	{
	public:
		static constexpr uint32 WIDTH = 800;
		static constexpr uint32 HEIGHT = 600;
	public:
		FirstApp();
		void Run();
	private:
		HDNWindow m_Window{ WIDTH, HEIGHT, "First App"};
		HDNDevice m_Device{ m_Window };
		HDNPipeline m_Pipeline{ 
			&m_Device, 
			"Shaders/simple_shader.vert.spv", 
			"Shaders/simple_shader.frag.spv", 
			HDNPipeline::DefaultPipelineConfigInfo(WIDTH, HEIGHT)
		};
	};
}