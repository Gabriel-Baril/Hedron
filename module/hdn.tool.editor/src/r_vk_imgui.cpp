#include "r_vk_imgui.h"

#include "core/core.h"
#include "font_awesome_icons.h"

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

	void ImguiSystem::Init(
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

		CreateFramebuffer();
		CreateImguiContext();
		SetStyle();
		SetupBackend(&m_MainWindowData);
		// Load custom font
	}

	void ImguiSystem::BeginFrame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// ImGui::DockSpaceOverViewport();
	}

	void ImguiSystem::EndFrame(const ImVec4& clearColor, VkCommandBuffer commandBuffer) // ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
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

	void ImguiSystem::Shutdown()
	{
		VkResult err = vkDeviceWaitIdle(m_Device);
		check_vk_result(err);
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		ImGui_ImplVulkanH_DestroyWindow(m_Instance, m_Device, &m_MainWindowData, m_Allocator);
	}

	void ImguiSystem::CreateFramebuffer()
	{
		int w, h;
		glfwGetFramebufferSize(m_GLFWWindow, &w, &h);
		SetupVulKanWindow(&m_MainWindowData, m_Surface, w, h);
	}

	void ImguiSystem::SetupVulKanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height)
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

	void ImguiSystem::CreateImguiContext()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.Fonts->AddFontDefault();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	}

	void ImguiSystem::SetStyle()
	{
		ImGuiIO& io = ImGui::GetIO();
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF("fonts/FontAwesome/FontAwesome-Regular.ttf", 12.0f, &icons_config, icons_ranges);
		io.Fonts->AddFontFromFileTTF("fonts/OpenSans/OpenSans-Bold.ttf", 18.0f);

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame Background
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		//Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}

	void ImguiSystem::SetupBackend(ImGui_ImplVulkanH_Window* wd)
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

	void ImguiSystem::FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data)
	{
		MAYBE_UNUSED(wd);
		MAYBE_UNUSED(draw_data);
		// Record dear imgui primitives into command buffer
	}

	void ImguiSystem::FramePresent(ImGui_ImplVulkanH_Window* wd)
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
