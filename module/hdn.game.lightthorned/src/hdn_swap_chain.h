#pragma once

#include "hdn_device.h"

#include "core/stl/vector.h"

// vulkan headers
#include <vulkan/vulkan.h>

namespace hdn {

	class HDNSwapChain {
	public:
		static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

		// TODO: Convert deviceRef to ptr
		HDNSwapChain(HDNDevice& deviceRef, VkExtent2D windowExtent);
		HDNSwapChain(HDNDevice& deviceRef, VkExtent2D windowExtent, std::shared_ptr<HDNSwapChain> previous);
		~HDNSwapChain();

		HDNSwapChain(const HDNSwapChain&) = delete;
		HDNSwapChain& operator=(const HDNSwapChain&) = delete;
		HDNSwapChain(HDNSwapChain&&) = delete;
		HDNSwapChain& operator=(HDNSwapChain&&) = delete;

		VkFramebuffer GetFrameBuffer(int index) { return m_SwapChainFramebuffers[index]; }
		VkRenderPass GetRenderPass() { return m_RenderPass; }
		VkImageView GetImageView(int index) { return m_SwapChainImageViews[index]; }
		size_t GetImageCount() { return m_SwapChainImages.size(); }
		VkFormat GetSwapChainImageFormat() { return m_SwapChainImageFormat; }
		VkExtent2D GetSwapChainExtent() { return m_SwapChainExtent; }
		uint32_t GetWidth() { return m_SwapChainExtent.width; }
		uint32_t GetHeight() { return m_SwapChainExtent.height; }

		float ExtentAspectRatio() {
			return static_cast<float>(m_SwapChainExtent.width) / static_cast<float>(m_SwapChainExtent.height);
		}
		VkFormat FindDepthFormat();

		VkResult AcquireNextImage(uint32_t* imageIndex);
		VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

		bool CompareSwapFormat(const HDNSwapChain& swapChain) const
		{
			return swapChain.m_SwapChainDepthFormat == m_SwapChainDepthFormat && 
				   swapChain.m_SwapChainImageFormat == m_SwapChainImageFormat;
		}

	private:
		void Init();
		void CreateSwapChain();
		void CreateImageViews();
		void CreateDepthResources();
		void CreateRenderPass();
		void CreateFramebuffers();
		void CreateSyncObjects();
		// Helper functions
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
			const vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(
			const vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	private:
		VkFormat m_SwapChainImageFormat;
		VkFormat m_SwapChainDepthFormat;
		VkExtent2D m_SwapChainExtent;

		vector<VkFramebuffer> m_SwapChainFramebuffers;
		VkRenderPass m_RenderPass;

		vector<VkImage> m_DepthImages;
		vector<VkDeviceMemory> m_DepthImageMemorys;
		vector<VkImageView> m_DepthImageViews;
		vector<VkImage> m_SwapChainImages;
		vector<VkImageView> m_SwapChainImageViews;

		HDNDevice& m_Device;
		VkExtent2D m_WindowExtent;

		VkSwapchainKHR m_SwapChain;
		std::shared_ptr<HDNSwapChain> m_OldSwapChain;

		vector<VkSemaphore> m_ImageAvailableSemaphores;
		vector<VkSemaphore> m_RenderFinishedSemaphores;
		vector<VkFence> m_InFlightFences;
		vector<VkFence> m_ImagesInFlight;
		size_t m_CurrentFrame = 0;
	};

}  // namespace lve
