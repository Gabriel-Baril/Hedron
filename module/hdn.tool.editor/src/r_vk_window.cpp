#include "r_vk_window.h"

#include "core/event/events.h"

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
		m_Data.self = this;

		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		glfwSetFramebufferSizeCallback(m_Window, VulkanWindow::FramebufferResizeCallback);

		SetWindowResizeCallback();
		SetWindowCloseCallback();
		SetKeyboardCallback();
		SetCharCallback();
		SetMouseButtonCallback();
		SetScrollCallback();
		SetMouseMovedCallback();
	}

	void VulkanWindow::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		WindowData& window_data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
		window_data.self->m_FramebufferResized = true;
		window_data.self->m_Width = width;
		window_data.self->m_Height = height;
	}

	void VulkanWindow::SetWindowResizeCallback()
	{
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);
				window_data.Width = width;
				window_data.Height = height;

				WindowResizedEvent window_resized_event(width, height);
				window_data.EventCallback(window_resized_event);
			});
	}

	// Binds our callback function to Window Close events from GLFW
	void VulkanWindow::SetWindowCloseCallback()
	{
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowClosedEvent window_closed_event;
				window_data.EventCallback(window_closed_event);
			});
	}

	void VulkanWindow::SetKeyboardCallback()
	{
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key_code, int scan_code, int action, int mods)
			{
				WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyCode code = static_cast<KeyCode>(key_code);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent key_pressed_event(code, 0);
					window_data.EventCallback(key_pressed_event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent key_released_event(code);
					window_data.EventCallback(key_released_event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent key_pressed_event(code, 1);
					window_data.EventCallback(key_pressed_event);
					break;
				}
				}
			});
	}

	void VulkanWindow::SetCharCallback()
	{
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t key_code)
			{
				WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyCode code = static_cast<KeyCode>(key_code);
				KeyTypedEvent key_typed_event(code);
				window_data.EventCallback(key_typed_event);
			});
	}

	void VulkanWindow::SetMouseButtonCallback()
	{
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseButton buttonCode = static_cast<MouseButton>(button);
				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent mouse_button_pressed_event(buttonCode);
					window_data.EventCallback(mouse_button_pressed_event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent mouse_button_released_event(buttonCode);
					window_data.EventCallback(mouse_button_released_event);
					break;
				}
				}
			});
	}

	void VulkanWindow::SetScrollCallback()
	{
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double x_offset, double y_offset)
			{
				WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent mouse_scrolled_event((f32)x_offset, (f32)y_offset);
				window_data.EventCallback(mouse_scrolled_event);
			});
	}

	void VulkanWindow::SetMouseMovedCallback()
	{
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x_pos, double y_pos)
			{
				WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent mouse_moved_event((f32)x_pos, (f32)y_pos);
				window_data.EventCallback(mouse_moved_event);
			});
	}
}
