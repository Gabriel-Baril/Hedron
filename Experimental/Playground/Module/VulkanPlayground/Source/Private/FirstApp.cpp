#include "VulkanPlayground/FirstApp.h"

#include <stdexcept>
#include <array>

namespace hdn
{
	FirstApp::FirstApp()
	{
		CreatePipelineLayout();
		CreatePipeline();
		CreateCommandBuffers();
	}

	FirstApp::~FirstApp()
	{
		vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
	}

	void FirstApp::Run()
	{
		while (!m_Window.ShouldClose())
		{
			glfwPollEvents();
			DrawFrame();
		}

		vkDeviceWaitIdle(m_Device.device());
	}

	void FirstApp::CreatePipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr; // Used to pass data other than our vertex data, to our vertex and fragment shader. For example, texture and uniform buffer.
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // A way to push a very small amount of data to our shader
		if (vkCreatePipelineLayout(m_Device.device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout");
		}
	}

	void FirstApp::CreatePipeline()
	{
		auto pipelineConfig = HDNPipeline::DefaultPipelineConfigInfo(m_Swapchain.width(), m_Swapchain.height());
		pipelineConfig.renderPass = m_Swapchain.getRenderPass(); // A render pass describe the structure and format of our framebuffer objects and their attachments
		pipelineConfig.pipelineLayout = m_PipelineLayout;
		m_Pipeline = std::make_unique<HDNPipeline>(&m_Device, "Shaders/simple_shader.vert.spv", "Shaders/simple_shader.frag.spv", pipelineConfig);
	}

	void FirstApp::CreateCommandBuffers()
	{
		m_CommandBuffers.resize(m_Swapchain.imageCount()); // Likely 2 (double buffering) or 3 (triple buffering)
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; // Primary = Can be submitted to a queue for execution, but cannot be called by other command buffer
		allocInfo.commandPool = m_Device.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32>(m_CommandBuffers.size());
		if (vkAllocateCommandBuffers(m_Device.device(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffer");
		}

		for (int i = 0;i < m_CommandBuffers.size(); i++)
		{
			VkCommandBuffer& currentCommandBuffer = m_CommandBuffers.at(i);

			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			if (vkBeginCommandBuffer(currentCommandBuffer, &beginInfo) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to begin recording command buffer");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = m_Swapchain.getRenderPass();
			renderPassInfo.framebuffer = m_Swapchain.getFrameBuffer(i);
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = m_Swapchain.getSwapChainExtent();

			std::array<VkClearValue, 2> clearValues{}; // Control what should be the initial value of our attachment
			clearValues[0].color = { 0.1f, 0.1f, 0.1f, 0.1f };
			clearValues[1].depthStencil = { 1.0f, 0 };
			renderPassInfo.clearValueCount = static_cast<uint32>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(currentCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE); // VK_SUBPASS_CONTENTS_INLINE = Signals that the subsequent render pass command will be directly embedded in the primary command buffer itself
			m_Pipeline->Bind(currentCommandBuffer);
			vkCmdDraw(currentCommandBuffer, 3, 1, 0, 0); // Instances can be used if you want to draw multiple copy of the same vertex data
			vkCmdEndRenderPass(currentCommandBuffer);

			if (vkEndCommandBuffer(currentCommandBuffer) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to end command buffer");
			}
		}
	}

	void FirstApp::DrawFrame()
	{
		uint32 imageIndex;
		auto result = m_Swapchain.acquireNextImage(&imageIndex); // Fetch the index of the next image to be used for rendering
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire the next swapchain image");
		}
		result = m_Swapchain.submitCommandBuffers(&m_CommandBuffers.at(imageIndex), &imageIndex); // It will submit command buffer to the device graphics queue while handling cpu and gpu synchronization
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to present swapchain image");
		}
	}
}
