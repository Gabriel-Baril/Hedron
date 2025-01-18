
#pragma once

#include "hdn_device.h"
#include "hdn_window.h"
#include "hdn_swap_chain.h"
#include "hdn_model.h"

#include "core/stl/vector.h"

namespace hdn
{

	class HDNRenderer
	{
	public:
		HDNRenderer(HDNWindow* window, HDNDevice* device);
		virtual ~HDNRenderer();
		HDNRenderer(const HDNRenderer&) = delete;
		HDNRenderer& operator=(const HDNRenderer&) = delete;

		VkRenderPass GetSwapChainRenderPass() const { return m_Swapchain->GetRenderPass(); }
		f32 GetAspectRatio() const { return m_Swapchain->ExtentAspectRatio(); }
		bool IsFrameInProgress() const { return m_IsFrameStarted; }

		VkCommandBuffer GetCurrentCommandBuffer() const
		{
			assert(IsFrameInProgress() && "Cannot get command buffer when frame not in progress");
			return m_CommandBuffers.at(m_CurrentFrameIndex);
		}

		int GetFrameIndex() const
		{
			assert(IsFrameInProgress() && "Cannot get frame index when frame not in progress");
			return m_CurrentFrameIndex;
		}

		VkCommandBuffer BeginFrame();
		void EndFrame();
		void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);
	private:
		void CreateCommandBuffers();
		void FreeCommandBuffers();
		void RecreateSwapchain();
	private:
		HDNWindow* m_Window;
		HDNDevice* m_Device;
		Scope<HDNSwapChain> m_Swapchain;
		vector<VkCommandBuffer> m_CommandBuffers;

		u32 m_CurrentImageIndex = 0;
		i32 m_CurrentFrameIndex = 0;
		bool m_IsFrameStarted = false;
	};
}