#include "r_vk_renderer.h"

#include "core/core.h"
#include "core/stl/array.h"

namespace hdn
{
	VulkanRenderer::VulkanRenderer(const ApplicationConfig& config)
	{
		m_Window = make_ref<VulkanWindow>(config.windowWidth, config.windowHeight, config.applicationName);
		m_Device = make_ref<VulkanDevice>(m_Window);

		recreate_swapchain();
		create_command_buffers();
	}

	VulkanRenderer::~VulkanRenderer()
	{
		free_command_buffers();
	}

	VkCommandBuffer VulkanRenderer::begin_frame()
	{
		assert(!is_frame_in_progress());
		auto result = m_Swapchain->acquire_next_image(&m_CurrentImageIndex); // Fetch the index of the next image to be used for rendering

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreate_swapchain();
			return nullptr;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			HTHROW(std::runtime_error, "Failed to acquire the next swapchain image");
		}
		m_IsFrameStarted = true;
		auto commandBuffer = get_current_command_buffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			HTHROW(std::runtime_error, "Failed to begin recording command buffer");
		}
		return commandBuffer;
	}

	void VulkanRenderer::end_frame()
	{
		assert(is_frame_in_progress());
		auto commandBuffer = get_current_command_buffer();
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			HTHROW(std::runtime_error, "Failed to end command buffer");
		}
		auto result = m_Swapchain->submit_command_buffers(&commandBuffer, &m_CurrentImageIndex); // It will submit command buffer to the device graphics queue while handling cpu and gpu synchronization
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_Window->was_window_resized())
		{
			m_Window->reset_window_resized_flag();
			recreate_swapchain();
		}
		else if (result != VK_SUCCESS)
		{
			HTHROW(std::runtime_error, "Failed to present swapchain image");
		}

		m_IsFrameStarted = false;
		m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanRenderer::begin_swap_chain_render_pass(VkCommandBuffer commandBuffer)
	{
		assert(is_frame_in_progress());
		assert(commandBuffer == get_current_command_buffer() && "Can't begin render pass on command buffer from a different frame");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_Swapchain->get_render_pass();
		renderPassInfo.framebuffer = m_Swapchain->get_frame_buffer(m_CurrentImageIndex);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_Swapchain->get_swap_chain_extent();

		array<VkClearValue, 2> clearValues{}; // Control what should be the initial value of our attachment
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 0.1f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<u32>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE); // VK_SUBPASS_CONTENTS_INLINE = Signals that the subsequent render pass command will be directly embedded in the primary command buffer itself

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_Swapchain->get_swap_chain_extent().width);
		viewport.height = static_cast<float>(m_Swapchain->get_swap_chain_extent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor({ 0, 0 }, m_Swapchain->get_swap_chain_extent());
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	}

	void VulkanRenderer::end_swap_chain_render_pass(VkCommandBuffer commandBuffer)
	{
		assert(is_frame_in_progress());
		assert(commandBuffer == get_current_command_buffer() && "Can't end render pass on command buffer from a different frame");

		vkCmdEndRenderPass(commandBuffer);
	}

	void VulkanRenderer::create_command_buffers()
	{
		m_CommandBuffers.resize(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT); // Likely 2 (double buffering) or 3 (triple buffering)
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; // Primary = Can be submitted to a queue for execution, but cannot be called by other command buffer
		allocInfo.commandPool = m_Device->get_command_pool();
		allocInfo.commandBufferCount = static_cast<u32>(m_CommandBuffers.size());
		if (vkAllocateCommandBuffers(m_Device->get_device(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
		{
			HTHROW(std::runtime_error, "Failed to allocate command buffer");
		}

	}

	void VulkanRenderer::free_command_buffers()
	{
		vkFreeCommandBuffers(m_Device->get_device(), m_Device->get_command_pool(), static_cast<u32>(m_CommandBuffers.size()), m_CommandBuffers.data());
		m_CommandBuffers.clear();
	}

	void VulkanRenderer::recreate_swapchain()
	{
		auto extent = m_Window->get_extent(); // Get the current window size
		while (extent.width == 0 || extent.height == 0)
		{
			extent = m_Window->get_extent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(m_Device->get_device()); // This is an easy way that we can wait until the current swapchain is no longer being used

		if (m_Swapchain == nullptr)
		{
			m_Swapchain = make_scope<VulkanSwapChain>(*m_Device, extent);
		}
		else
		{
			Ref<VulkanSwapChain> oldSwapChain = std::move(m_Swapchain);
			m_Swapchain = make_scope<VulkanSwapChain>(*m_Device, extent, oldSwapChain);

			if (!oldSwapChain->compare_swap_format(*m_Swapchain.get()))
			{
				HTHROW(std::runtime_error, "Swap chain image(or depth) format has changed");
			}
		}

		// If render pass compatible do nothing else
		// CreatePipeline();
	}
}
