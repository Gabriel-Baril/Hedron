#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "core/core.h"
#include "core/event/event.h"

namespace hdn
{
	class VulkanWindow
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		inline virtual void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

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

		void SetWindowResizeCallback();
		void SetWindowCloseCallback();
		void SetKeyboardCallback();
		void SetCharCallback();
		void SetMouseButtonCallback();
		void SetScrollCallback();
		void SetMouseMovedCallback();
	private:
		struct WindowData
		{
			std::string Title;
			uint32_t Width;
			uint32_t Height;
			bool VSync;
			EventCallbackFn EventCallback;
			VulkanWindow* self;
		};
		WindowData m_Data;

		u32 m_Width;
		u32 m_Height;
		bool m_FramebufferResized = false;
		
		string m_WindowName;
		GLFWwindow* m_Window = nullptr;
	};
}