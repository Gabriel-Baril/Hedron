#include "hdrpch.h"

#include "Platform/Windows/WindowsWindow.h"

#include "Hedron/Events/ApplicationEvents.h"
#include "Hedron/Events/KeyboardEvents.h"
#include "Hedron/Events/MouseEvents.h"

#include "Platform/OpenGL/OpenGLContext.h"



namespace Hedron
{
	static int s_GLFWWindowCount = 0;

	static void GLFW_error_callback(int errorCode, const char* errorDescription)
	{
		HDR_CORE_ERROR("GLFW Error ({0}): {1}", errorCode, errorDescription);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		HDR_PROFILE_FUNCTION();
		this->init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		this->shutdown();
	}

	void WindowsWindow::init(const WindowProps& props)
	{
		HDR_PROFILE_FUNCTION();
		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;

		HDR_CORE_INFO("Creating window {0} [{1}, {2}]", props.title, props.width, props.height);

		// We might create multiple window so we need to make sure that we initialise GLFW only once
		if (s_GLFWWindowCount == 0)
		{
			// TODO: glfwTerminate on system shutdown
			HDR_PROFILE_SCOPE("glfwInit");
			int success = glfwInit();
			
			HDR_CORE_ASSERT(success, "Could not initialize GLFW !");
			glfwSetErrorCallback(GLFW_error_callback);
		}

		{
			HDR_PROFILE_SCOPE("glfwCreateWindow");
			m_window = glfwCreateWindow((int)props.width, (int)props.height, props.title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		m_context = new OpenGLContext(m_window);
		m_context->init();

		glfwSetWindowUserPointer(m_window, &m_data);
		this->set_v_sync(false);

		// Set GLFW callbacks
		this->set_window_resize_callback();
		this->set_window_close_callback();
		this->set_keyboard_callback();
		this->set_char_callback();
		this->set_mouse_button_callback();
		this->set_scroll_callback();
		this->set_mouse_moved_callback();

	}

	void WindowsWindow::on_update()
	{
		HDR_PROFILE_FUNCTION();

		glfwPollEvents();
		m_context->swap_buffers();
	}

	void WindowsWindow::set_v_sync(bool enabled)
	{
		HDR_PROFILE_FUNCTION();

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
		HDR_PROFILE_FUNCTION();
		glfwDestroyWindow(m_window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}

	}

	// Binds our callback function to Window Resize events from GLFW
	void WindowsWindow::set_window_resize_callback()
	{
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			windowData.width = width;
			windowData.height = height;

			WindowResizeEvent windowResizeEvent(width, height);
			windowData.eventCallback(windowResizeEvent);
		});
	}

	// Binds our callback function to Window Close events from GLFW
	void WindowsWindow::set_window_close_callback()
	{
		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent windowCloseEvent;
			windowData.eventCallback(windowCloseEvent);
		});
	}

	void WindowsWindow::set_keyboard_callback()
	{
		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int keyCode, int scanCode, int action, int mods)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent keyPressedEvent(keyCode, 0);
					windowData.eventCallback(keyPressedEvent);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent keyReleasedEvent(keyCode);
					windowData.eventCallback(keyReleasedEvent);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent keyPressedEvent(keyCode, 1);
					windowData.eventCallback(keyPressedEvent);
					break;
				}
			}
		});
	}

	void WindowsWindow::set_char_callback()
	{
		glfwSetCharCallback(m_window, [](GLFWwindow* window, uint32_t keyCode)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent keyTypedEvent(keyCode);
			windowData.eventCallback(keyTypedEvent);
		});
	}

	void WindowsWindow::set_mouse_button_callback()
	{
		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent mouseButtonPressedEvent(button);
				windowData.eventCallback(mouseButtonPressedEvent);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent mouseButtonReleasedEvent(button);
				windowData.eventCallback(mouseButtonReleasedEvent);
				break;
			}
			}
		});
	}

	void WindowsWindow::set_scroll_callback()
	{
		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent mouseScrolledEvent((float)xOffset, (float)yOffset);
			windowData.eventCallback(mouseScrolledEvent);
		});
	}

	void WindowsWindow::set_mouse_moved_callback()
	{
		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent mouseMovedEvent((float)xPos, (float)yPos);
			windowData.eventCallback(mouseMovedEvent);
		});
	}
}