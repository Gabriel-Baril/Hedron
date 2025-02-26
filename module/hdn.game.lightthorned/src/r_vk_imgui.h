#pragma once

#include "r_vk_window.h"
#include "r_vk_device.h"
#include "r_vk_swapchain.h"

#include "imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_vulkan.h"

namespace hdn
{
	class ImguiSystem
	{
	public:
		ImguiSystem() = default;

		void Init(
			GLFWwindow* glfwWindow,
			VkSurfaceKHR surface,
			VkInstance instance,
			VkPhysicalDevice physicalDevice,
			VkDevice device,
			uint32_t queueFamilyGraphics,
			VkQueue graphicsQueue,
			VkDescriptorPool descriptorPool
		);

		void BeginFrame();
		void EndFrame(const ImVec4& clearColor, VkCommandBuffer commandBuffer);
		void Shutdown();
	private:
		void CreateFramebuffer();
		void SetupVulKanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);

		void CreateImguiContext();
		void SetStyle();
		void SetupBackend(ImGui_ImplVulkanH_Window* wd);

		void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
		void FramePresent(ImGui_ImplVulkanH_Window* wd);
	private:
		GLFWwindow* m_GLFWWindow = nullptr;
		VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

		VkAllocationCallbacks*   m_Allocator = nullptr;
		VkInstance               m_Instance = VK_NULL_HANDLE; // Used same
		VkPhysicalDevice         m_PhysicalDevice = VK_NULL_HANDLE; // Used same
		VkDevice                 m_Device = VK_NULL_HANDLE; // Used same
		uint32_t                 m_QueueFamily = (uint32_t)-1;
		VkQueue                  m_Queue = VK_NULL_HANDLE;
		// VkDebugReportCallbackEXT m_DebugReport = VK_NULL_HANDLE;
		VkPipelineCache          m_PipelineCache = VK_NULL_HANDLE;
		VkDescriptorPool         m_DescriptorPool = VK_NULL_HANDLE;
		
		ImGui_ImplVulkanH_Window m_MainWindowData;
		int                      m_MinImageCount = 2; // Used the value from swapchain. MAX_FRAMES_IN_FLIGHT??
		bool                     m_SwapChainRebuild = false;
	};
}