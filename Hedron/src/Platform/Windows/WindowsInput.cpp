#include "hdrpch.h"

#include <GLFW/glfw3.h>

#include "Hedron/Core/Application.h"
#include "Hedron/Core/Input.h"

namespace Hedron
{
	bool Input::is_key_pressed(int keyCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::get().get_window().get_native_window());
		auto state = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::is_key_pressed(KeyCode keyCode)
	{
		return Input::is_key_pressed(static_cast<int32_t>(keyCode));
	}

	bool Input::is_mouse_button_pressed(int mouseButton)
	{
		auto window = static_cast<GLFWwindow*>(Application::get().get_window().get_native_window());
		auto state = glfwGetMouseButton(window, mouseButton);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::get_mouse_position()
	{
		auto window = static_cast<GLFWwindow*>(Application::get().get_window().get_native_window());
		double mouse_x, mouse_y;
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		return std::make_pair<float, float>((float)mouse_x, (float)mouse_y);
	}

	float Input::get_mouse_x()
	{
		auto [mouse_x, mouse_y] = Input::get_mouse_position();
		return mouse_x;
	}

	float Input::get_mouse_y()
	{
		auto [mouse_x, mouse_y] = Input::get_mouse_position();
		return mouse_y;
	}
}