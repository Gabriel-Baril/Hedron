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

		bool should_close();
		VkExtent2D get_extent() { return { m_Width, m_Height }; }
		bool was_window_resized() { return m_FramebufferResized; }
		void reset_window_resized_flag() { m_FramebufferResized = false; }
		GLFWwindow* get_glfw_window() const { return m_Window; }

		void create_window_surface(VkInstance instance, VkSurfaceKHR* surface);
	private:
		void init_window();
		static void framebuffer_resize_callback(GLFWwindow* window, int width, int height);
	private:
		u32 m_Width;
		u32 m_Height;
		bool m_FramebufferResized = false;
		
		string m_WindowName;
		GLFWwindow* m_Window = nullptr;
	};
}