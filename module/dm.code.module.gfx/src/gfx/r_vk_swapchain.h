#pragma once

#include "r_vk_device.h"

#include "core/stl/vector.h"

// vulkan headers
#include <vulkan/vulkan.h>

namespace dm
{

	class VulkanSwapChain
	{
	public:
		static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

		// TODO: Convert deviceRef to ptr
		VulkanSwapChain(VulkanDevice &deviceRef, VkExtent2D windowExtent);
		VulkanSwapChain(VulkanDevice &deviceRef, VkExtent2D windowExtent, std::shared_ptr<VulkanSwapChain> previous);
		~VulkanSwapChain();

		VulkanSwapChain(const VulkanSwapChain &) = delete;
		VulkanSwapChain &operator=(const VulkanSwapChain &) = delete;
		VulkanSwapChain(VulkanSwapChain &&) = delete;
		VulkanSwapChain &operator=(VulkanSwapChain &&) = delete;

		VkFramebuffer get_frame_buffer(int index) { return m_SwapChainFramebuffers[index]; }
		VkRenderPass get_render_pass() { return m_RenderPass; }
		VkImageView get_image_view(int index) { return m_SwapChainImageViews[index]; }
		size_t get_image_count() { return m_SwapChainImages.size(); }
		VkFormat get_swap_chain_image_format() { return m_SwapChainImageFormat; }
		VkExtent2D get_swap_chain_extent() { return m_SwapChainExtent; }
		uint32_t get_width() { return m_SwapChainExtent.width; }
		uint32_t get_height() { return m_SwapChainExtent.height; }

		float extent_aspect_ratio()
		{
			return static_cast<float>(m_SwapChainExtent.width) / static_cast<float>(m_SwapChainExtent.height);
		}
		VkFormat find_depth_format();

		VkResult acquire_next_image(uint32_t *imageIndex);
		VkResult submit_command_buffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

		bool compare_swap_format(const VulkanSwapChain &swapChain) const
		{
			return swapChain.m_SwapChainDepthFormat == m_SwapChainDepthFormat &&
						 swapChain.m_SwapChainImageFormat == m_SwapChainImageFormat;
		}

	private:
		void init();
		void create_swap_chain();
		void create_image_views();
		void create_depth_resources();
		void create_render_pass();
		void create_framebuffers();
		void create_sync_objects();
		// Helper functions
		VkSurfaceFormatKHR choose_swap_surface_format(
				const vector<VkSurfaceFormatKHR> &availableFormats);
		VkPresentModeKHR choose_swap_present_mode(
				const vector<VkPresentModeKHR> &availablePresentModes);
		VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR &capabilities);

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

		VulkanDevice &m_Device;
		VkExtent2D m_WindowExtent;

		VkSwapchainKHR m_SwapChain;
		std::shared_ptr<VulkanSwapChain> m_OldSwapChain;

		vector<VkSemaphore> m_ImageAvailableSemaphores;
		vector<VkSemaphore> m_RenderFinishedSemaphores;
		vector<VkFence> m_InFlightFences;
		vector<VkFence> m_ImagesInFlight;
		size_t m_CurrentFrame = 0;
	};

} // namespace lve
