#include "HDNRenderer.h"

#include "core/Core.h"

#include <stdexcept>
#include <array>

namespace hdn
{
	HDNRenderer::HDNRenderer(HDNWindow* window, HDNDevice* device)
		: m_Window{window}, m_Device{device}
	{
		RecreateSwapchain();
		CreateCommandBuffers();
	}

	HDNRenderer::~HDNRenderer()
	{
		FreeCommandBuffers();
	}

	VkCommandBuffer HDNRenderer::BeginFrame()
	{
		assert(!IsFrameInProgress());
		auto result = m_Swapchain->AcquireNextImage(&m_CurrentImageIndex); // Fetch the index of the next image to be used for rendering

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapchain();
			return nullptr;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire the next swapchain image");
		}
		m_IsFrameStarted = true;
		auto commandBuffer = GetCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin recording command buffer");
		}
		return commandBuffer;
	}

	void HDNRenderer::EndFrame()
	{
		assert(IsFrameInProgress());
		auto commandBuffer = GetCurrentCommandBuffer();
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to end command buffer");
		}
		auto result = m_Swapchain->SubmitCommandBuffers(&commandBuffer, &m_CurrentImageIndex); // It will submit command buffer to the device graphics queue while handling cpu and gpu synchronization
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_Window->WasWindowResized())
		{
			m_Window->ResetWindowResizedFlag();
			RecreateSwapchain();
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to present swapchain image");
		}

		m_IsFrameStarted = false;
		m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % HDNSwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void HDNRenderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(IsFrameInProgress());
		assert(commandBuffer == GetCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_Swapchain->GetRenderPass();
		renderPassInfo.framebuffer = m_Swapchain->GetFrameBuffer(m_CurrentImageIndex);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_Swapchain->GetSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{}; // Control what should be the initial value of our attachment
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 0.1f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<u32>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE); // VK_SUBPASS_CONTENTS_INLINE = Signals that the subsequent render pass command will be directly embedded in the primary command buffer itself

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_Swapchain->GetSwapChainExtent().width);
		viewport.height = static_cast<float>(m_Swapchain->GetSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor({ 0, 0 }, m_Swapchain->GetSwapChainExtent());
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	}

	void HDNRenderer::EndSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(IsFrameInProgress());
		assert(commandBuffer == GetCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");

		vkCmdEndRenderPass(commandBuffer);
	}

	void HDNRenderer::CreateCommandBuffers()
	{
		m_CommandBuffers.resize(HDNSwapChain::MAX_FRAMES_IN_FLIGHT); // Likely 2 (double buffering) or 3 (triple buffering)
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; // Primary = Can be submitted to a queue for execution, but cannot be called by other command buffer
		allocInfo.commandPool = m_Device->GetCommandPool();
		allocInfo.commandBufferCount = static_cast<u32>(m_CommandBuffers.size());
		if (vkAllocateCommandBuffers(m_Device->GetDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffer");
		}

	}

	void HDNRenderer::FreeCommandBuffers()
	{
		vkFreeCommandBuffers(m_Device->GetDevice(), m_Device->GetCommandPool(), static_cast<u32>(m_CommandBuffers.size()), m_CommandBuffers.data());
		m_CommandBuffers.clear();
	}

	void HDNRenderer::RecreateSwapchain()
	{
		auto extent = m_Window->GetExtent(); // Get the current window size
		while (extent.width == 0 || extent.height == 0)
		{
			extent = m_Window->GetExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(m_Device->GetDevice()); // This is an easy way that we can wait until the current swapchain is no longer being used

		if (m_Swapchain == nullptr)
		{
			m_Swapchain = CreateScope<HDNSwapChain>(*m_Device, extent);
		}
		else
		{
			Ref<HDNSwapChain> oldSwapChain = std::move(m_Swapchain);
			m_Swapchain = CreateScope<HDNSwapChain>(*m_Device, extent, oldSwapChain);

			if (!oldSwapChain->CompareSwapFormat(*m_Swapchain.get()))
			{
				throw std::runtime_error("Swap chain image(or depth) format has changed");
			}
		}

		// If render pass compatible do nothing else
		// CreatePipeline();
	}
}
