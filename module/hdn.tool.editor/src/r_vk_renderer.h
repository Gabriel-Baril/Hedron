
#pragma once

#include "r_vk_device.h"
#include "r_vk_window.h"
#include "r_vk_swapchain.h"
#include "r_vk_model.h"

#include "core/stl/vector.h"

namespace hdn
{

	class VulkanRenderer
	{
	public:
		VulkanRenderer(VulkanWindow* window, VulkanDevice* device);
		virtual ~VulkanRenderer();
		VulkanRenderer(const VulkanRenderer&) = delete;
		VulkanRenderer& operator=(const VulkanRenderer&) = delete;

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
		VulkanWindow* m_Window;
		VulkanDevice* m_Device;
		Scope<VulkanSwapChain> m_Swapchain;
		vector<VkCommandBuffer> m_CommandBuffers;

		u32 m_CurrentImageIndex = 0;
		i32 m_CurrentFrameIndex = 0;
		bool m_IsFrameStarted = false;
	};
}