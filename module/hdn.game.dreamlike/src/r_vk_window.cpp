#include "r_vk_window.h"

namespace hdn
{
	VulkanWindow::VulkanWindow(u32 width, u32 height, const string& name)
		: m_Width{ width }, m_Height{ height }, m_WindowName{ name }
	{
		InitWindow();
	}

	VulkanWindow::~VulkanWindow()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	bool VulkanWindow::ShouldClose()
	{
		return glfwWindowShouldClose(m_Window);
	}

	void VulkanWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, m_Window, nullptr, surface) != VK_SUCCESS)
		{
			HTHROW(std::runtime_error, "Failed to create window surface");
		}
	}

	void VulkanWindow::InitWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_Window, this);
		glfwSetFramebufferSizeCallback(m_Window, VulkanWindow::FramebufferResizeCallback);
	}

	void VulkanWindow::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto hdnWindow = reinterpret_cast<VulkanWindow*>(glfwGetWindowUserPointer(window));
		hdnWindow->m_FramebufferResized = true;
		hdnWindow->m_Width = width;
		hdnWindow->m_Height = height;
	}
}
