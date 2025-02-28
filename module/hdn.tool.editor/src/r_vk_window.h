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

		bool ShouldClose();
		VkExtent2D GetExtent() { return { m_Width, m_Height }; }
		bool WasWindowResized() { return m_FramebufferResized; }
		void ResetWindowResizedFlag() { m_FramebufferResized = false; }
		GLFWwindow* GetGLFWWindow() const { return m_Window; }

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	private:
		void InitWindow();
		static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

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