
#pragma once

#include "r_vk_device.h"
#include "r_vk_window.h"
#include "r_vk_swapchain.h"
#include "r_vk_model.h"

#include "core/stl/vector.h"

#include "core/application/application.h"

namespace dm
{

	class VulkanRenderer
	{
	public:
		VulkanRenderer(const ApplicationConfig &config);
		virtual ~VulkanRenderer();
		VulkanRenderer(const VulkanRenderer &) = delete;
		VulkanRenderer &operator=(const VulkanRenderer &) = delete;

		VkRenderPass get_swap_chain_render_pass() const { return m_Swapchain->get_render_pass(); }
		f32 get_aspect_ratio() const { return m_Swapchain->extent_aspect_ratio(); }
		bool is_frame_in_progress() const { return m_IsFrameStarted; }

		VkCommandBuffer get_current_command_buffer() const
		{
			assert(is_frame_in_progress() && "Cannot get command buffer when frame not in progress");
			return m_CommandBuffers.at(m_CurrentFrameIndex);
		}

		int get_frame_index() const
		{
			assert(is_frame_in_progress() && "Cannot get frame index when frame not in progress");
			return m_CurrentFrameIndex;
		}

		Ref<VulkanWindow> get_window() { return m_Window; }
		Ref<VulkanDevice> get_device() { return m_Device; }

		VkCommandBuffer begin_frame();
		void end_frame();
		void begin_swap_chain_render_pass(VkCommandBuffer commandBuffer);
		void end_swap_chain_render_pass(VkCommandBuffer commandBuffer);

	private:
		void create_command_buffers();
		void free_command_buffers();
		void recreate_swapchain();

	private:
		Ref<VulkanWindow> m_Window;
		Ref<VulkanDevice> m_Device;
		Scope<VulkanSwapChain> m_Swapchain;
		vector<VkCommandBuffer> m_CommandBuffers;

		u32 m_CurrentImageIndex = 0;
		i32 m_CurrentFrameIndex = 0;
		bool m_IsFrameStarted = false;
	};
}
