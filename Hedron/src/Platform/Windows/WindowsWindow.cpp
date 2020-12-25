#include "hdrpch.h"
#include "WindowsWindow.h"

namespace Hedron
{
	static bool s_GLFWInitialized = false;

	Window* Window::create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		this->init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		this->shutdown();
	}

	void WindowsWindow::init(const WindowProps& props)
	{
		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;

		HDR_CORE_INFO("Creating window {0} [{1}, {2}]", props.title, props.width, props.height);

		// We might create multiple window so we need to make sure that we initialise GLFW only once
		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			HDR_CORE_ASSERT(success, "Could not initialize GLFW !");

			s_GLFWInitialized = true;
		}

		m_window = glfwCreateWindow((int)props.width, (int)props.height, props.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_data);
		this->set_v_sync(true);
	}

	void WindowsWindow::on_update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void WindowsWindow::set_v_sync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_data.vSync = enabled;
	}

	bool WindowsWindow::is_v_sync() const
	{
		return m_data.vSync;
	}

	void WindowsWindow::shutdown()
	{
		glfwDestroyWindow(m_window);
	}
}