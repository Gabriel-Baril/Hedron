#include "r_vk_window.h"

namespace hdn
{
	VulkanWindow::VulkanWindow(u32 width, u32 height, const string& name)
		: m_Width{ width }, m_Height{ height }, m_WindowName{ name }
	{
		init_window();
	}

	VulkanWindow::~VulkanWindow()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	bool VulkanWindow::should_close()
	{
		return glfwWindowShouldClose(m_Window);
	}

	void VulkanWindow::create_window_surface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, m_Window, nullptr, surface) != VK_SUCCESS)
		{
			HTHROW(std::runtime_error, "Failed to create window surface");
		}
	}

	void VulkanWindow::init_window()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_Window, this);
		glfwSetFramebufferSizeCallback(m_Window, VulkanWindow::framebuffer_resize_callback);
	}

	void VulkanWindow::framebuffer_resize_callback(GLFWwindow* window, int width, int height)
	{
		auto hdnWindow = reinterpret_cast<VulkanWindow*>(glfwGetWindowUserPointer(window));
		hdnWindow->m_FramebufferResized = true;
		hdnWindow->m_Width = width;
		hdnWindow->m_Height = height;
	}
}
