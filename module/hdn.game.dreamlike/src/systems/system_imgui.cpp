#include "system_imgui.h"

#include "core/core.h"

#include "GLFW/glfw3.h"

namespace hdn
{
	// TODO: Refactor
	static void check_vk_result(VkResult err)
	{
		if (err == 0)
			return;
		fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
		if (err < 0)
			abort();
	}

	void ImguiSystem::init(
		GLFWwindow* glfwWindow, 
		VkSurfaceKHR surface, 
		VkInstance instance, 
		VkPhysicalDevice physicalDevice, 
		VkDevice device,
		uint32_t queueFamilyGraphics,
		VkQueue graphicsQueue,
		VkDescriptorPool descriptorPool
	)
	{
		assert(queueFamilyGraphics != (uint32_t)-1);

		m_GLFWWindow = glfwWindow;
		m_Surface = surface;
		m_Instance = instance;
		m_PhysicalDevice = physicalDevice;
		m_Device = device;
		m_QueueFamily = queueFamilyGraphics;
		m_Queue = graphicsQueue;
		m_DescriptorPool = descriptorPool;

		create_framebuffer();
		create_imgui_context();
		set_style();
		setup_backend(&m_MainWindowData);
		// Load custom font
	}

	void ImguiSystem::begin_frame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImguiSystem::end_frame(const ImVec4& clearColor, VkCommandBuffer commandBuffer) // ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	{
		ImGui::Render();
		ImDrawData* main_draw_data = ImGui::GetDrawData();
		const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);
		m_MainWindowData.ClearValue.color.float32[0] = clearColor.x * clearColor.w;
		m_MainWindowData.ClearValue.color.float32[1] = clearColor.y * clearColor.w;
		m_MainWindowData.ClearValue.color.float32[2] = clearColor.z * clearColor.w;
		m_MainWindowData.ClearValue.color.float32[3] = clearColor.w;
		if (!main_is_minimized)
			ImGui_ImplVulkan_RenderDrawData(main_draw_data, commandBuffer);

		// Update and Render additional Platform Windows
		ImGuiIO& io = ImGui::GetIO(); // TODO: Optimize
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void ImguiSystem::shutdown()
	{
		VkResult err = vkDeviceWaitIdle(m_Device);
		check_vk_result(err);
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		ImGui_ImplVulkanH_DestroyWindow(m_Instance, m_Device, &m_MainWindowData, m_Allocator);
	}

	void ImguiSystem::create_framebuffer()
	{
		int w, h;
		glfwGetFramebufferSize(m_GLFWWindow, &w, &h);
		setup_vulkan_window(&m_MainWindowData, m_Surface, w, h);
	}

	void ImguiSystem::setup_vulkan_window(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height)
	{
		wd->Surface = surface;

		// Check for WSI support
		VkBool32 res;
		vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, m_QueueFamily, wd->Surface, &res);
		if (res != VK_TRUE)
		{
			HERR("Error no WSI support on physical device 0");
			exit(-1);
		}

		// Select Surface Format
		const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
		const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(m_PhysicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

		// Select Present Mode
#ifdef APP_USE_UNLIMITED_FRAME_RATE
		VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
		VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
		wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(m_PhysicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
		//printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

		// Create SwapChain, RenderPass, Framebuffer, etc.
		IM_ASSERT(m_MinImageCount >= 2);
		ImGui_ImplVulkanH_CreateOrResizeWindow(m_Instance, m_PhysicalDevice, m_Device, wd, m_QueueFamily, m_Allocator, width, height, m_MinImageCount);
	}

	void ImguiSystem::create_imgui_context()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	}

	void ImguiSystem::set_style()
	{
		ImGui::StyleColorsDark();
	}

	void ImguiSystem::setup_backend(ImGui_ImplVulkanH_Window* wd)
	{
		ImGui_ImplGlfw_InitForVulkan(m_GLFWWindow, true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = m_Instance;
		init_info.PhysicalDevice = m_PhysicalDevice;
		init_info.Device = m_Device;
		init_info.QueueFamily = m_QueueFamily;
		init_info.Queue = m_Queue;
		init_info.PipelineCache = m_PipelineCache;
		init_info.DescriptorPool = m_DescriptorPool;
		init_info.RenderPass = wd->RenderPass;
		init_info.Subpass = 0;
		init_info.MinImageCount = m_MinImageCount;
		init_info.ImageCount = wd->ImageCount;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.Allocator = m_Allocator;
		init_info.CheckVkResultFn = check_vk_result;
		ImGui_ImplVulkan_Init(&init_info);
	}

	void ImguiSystem::frame_render(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data)
	{
		MAYBE_UNUSED(wd);
		MAYBE_UNUSED(draw_data);
		// Record dear imgui primitives into command buffer
	}

	void ImguiSystem::frame_present(ImGui_ImplVulkanH_Window* wd)
	{
		if (m_SwapChainRebuild)
			return;
		VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
		VkPresentInfoKHR info = {};
		info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &render_complete_semaphore;
		info.swapchainCount = 1;
		info.pSwapchains = &wd->Swapchain;
		info.pImageIndices = &wd->FrameIndex;
		VkResult err = vkQueuePresentKHR(m_Queue, &info);
		if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
		{
			m_SwapChainRebuild = true;
			return;
		}
		check_vk_result(err);
		wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->SemaphoreCount; // Now we can use the next set of semaphores
	}


}
