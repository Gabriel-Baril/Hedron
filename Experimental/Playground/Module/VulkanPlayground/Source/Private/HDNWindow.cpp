#include "VulkanPlayground/HDNWindow.h"

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

	void HDNWindow::InitWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowName.c_str(), nullptr, nullptr);
	}
}
