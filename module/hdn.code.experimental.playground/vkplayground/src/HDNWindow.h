#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "core/Core.h"
#include <string>

namespace hdn
{
	class HDNWindow
	{
	public:
		HDNWindow(uint32 width, uint32 height, const std::string& name);
		virtual ~HDNWindow();
		HDNWindow(const HDNWindow&) = delete;
		HDNWindow& operator=(const HDNWindow&) = delete;
		HDNWindow(HDNWindow&&) = delete;
		HDNWindow& operator=(HDNWindow&&) = delete;

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
		uint32 m_Width;
		uint32 m_Height;
		bool m_FramebufferResized = false;
		
		std::string m_WindowName;
		GLFWwindow* m_Window = nullptr;
	};
}