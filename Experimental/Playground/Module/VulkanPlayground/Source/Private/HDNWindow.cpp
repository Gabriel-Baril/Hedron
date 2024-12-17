#include "VulkanPlayground/HDNWindow.h"

#include <stdexcept>

namespace hdn
{
	HDNWindow::HDNWindow(uint32 width, uint32 height, const std::string& name)
		: m_Width{ width }, m_Height{ height }, m_WindowName{ name }
	{
		InitWindow();
	}

	HDNWindow::~HDNWindow()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	bool HDNWindow::ShouldClose()
	{
		return glfwWindowShouldClose(m_Window);
	}

	void HDNWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, m_Window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface");
		}
	}

	void HDNWindow::InitWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_Window, this);
		glfwSetFramebufferSizeCallback(m_Window, HDNWindow::FramebufferResizeCallback);
	}

	void HDNWindow::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto hdnWindow = reinterpret_cast<HDNWindow*>(glfwGetWindowUserPointer(window));
		hdnWindow->m_FramebufferResized = true;
		hdnWindow->m_Width = width;
		hdnWindow->m_Height = height;
	}
}
