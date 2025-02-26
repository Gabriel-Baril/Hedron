#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "core/core.h"

namespace hdn
{
	class VulkanWindow
	{
	public:
		VulkanWindow(u32 width, u32 height, const string& name);
		virtual ~VulkanWindow();
		VulkanWindow(const VulkanWindow&) = delete;
		VulkanWindow& operator=(const VulkanWindow&) = delete;
		VulkanWindow(VulkanWindow&&) = delete;
		VulkanWindow& operator=(VulkanWindow&&) = delete;

		bool ShouldClose();
		VkExtent2D GetExtent() { return { m_Width, m_Height }; }
		bool WasWindowResized() { return m_FramebufferResized; }
		void ResetWindowResizedFlag() { m_FramebufferResized = false; }
		GLFWwindow* GetGLFWWindow() const { return m_Window; }

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	private:
		void InitWindow();
		static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);
	private:
		u32 m_Width;
		u32 m_Height;
		bool m_FramebufferResized = false;
		
		string m_WindowName;
		GLFWwindow* m_Window = nullptr;
	};
}