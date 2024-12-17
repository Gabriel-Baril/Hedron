
#pragma once

#include "HDNDevice.h"
#include "HDNPipeline.h"
#include "HDNWindow.h"
#include "HDNSwapChain.h"
#include "HDNModel.h"

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
		FirstApp(FirstApp&&) = delete;
		FirstApp& operator=(FirstApp&&) = delete;

		void Run();
	private:
		void LoadModels();
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCommandBuffers();
		void FreeCommandBuffers();
		void DrawFrame();
		void RecreateSwapchain();
		void RecordCommandBuffer(int imageIndex);
	private:
		HDNWindow m_Window{ WIDTH, HEIGHT, "First App"};
		HDNDevice m_Device{ m_Window };
		std::unique_ptr<HDNSwapChain> m_Swapchain;
		std::unique_ptr<HDNPipeline> m_Pipeline; // TODO: Change to Scope<>
		VkPipelineLayout m_PipelineLayout;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		std::unique_ptr<HDNModel> m_Model;
	};
}