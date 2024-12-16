#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "Core/Core.h"
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

		bool ShouldClose();
		VkExtent2D GetExtent() { return { m_Width, m_Height }; }

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	private:
		void InitWindow();
	private:
		const uint32 m_Width;
		const uint32 m_Height;
		
		std::string m_WindowName;
		GLFWwindow* m_Window = nullptr;
	};
}