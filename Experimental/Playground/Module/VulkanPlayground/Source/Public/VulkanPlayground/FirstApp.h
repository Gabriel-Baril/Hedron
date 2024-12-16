
#pragma once

#include "HDNDevice.h"
#include "HDNPipeline.h"
#include "HDNWindow.h"
#include "HDNSwapChain.h"

#include <memory>
#include <vector>

namespace hdn
{
	class FirstApp
	{
	public:
		static constexpr uint32 WIDTH = 800;
		static constexpr uint32 HEIGHT = 600;
	public:
		FirstApp();
		virtual ~FirstApp();
		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;


		void Run();
	private:
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCommandBuffers();
		void DrawFrame();
	private:
		HDNWindow m_Window{ WIDTH, HEIGHT, "First App"};
		HDNDevice m_Device{ m_Window };
		HDNSwapChain m_Swapchain{ m_Device, m_Window.GetExtent() };
		std::unique_ptr<HDNPipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
		std::vector<VkCommandBuffer> m_CommandBuffers;
	};
}