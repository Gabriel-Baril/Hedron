#include "hdrpch.h"

#include <GLFW/glfw3.h>

#include "Hedron/Core/Application.h"
#include "Platform/Windows/WindowsInput.h"

namespace Hedron
{
	Input* Input::s_instance = new WindowsInput();

	bool WindowsInput::is_key_pressed_impl(int keyCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::get().get_window().get_native_window());
		auto state = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::is_mouse_button_pressed_impl(int mouseButton)
	{
		auto window = static_cast<GLFWwindow*>(Application::get().get_window().get_native_window());
		auto state = glfwGetMouseButton(window, mouseButton);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::get_mouse_position_impl()
	{
		auto window = static_cast<GLFWwindow*>(Application::get().get_window().get_native_window());
		double mouse_x, mouse_y;
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		return std::make_pair<float, float>((float)mouse_x, (float)mouse_y);
	}

	float WindowsInput::get_mouse_x_impl()
	{
		auto [mouse_x, mouse_y] = get_mouse_position_impl();
		return mouse_x;
	}

	float WindowsInput::get_mouse_y_impl()
	{
		auto [mouse_x, mouse_y] = get_mouse_position_impl();
		return mouse_y;
	}
}