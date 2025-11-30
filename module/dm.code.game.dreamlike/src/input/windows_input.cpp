#include "application.h"
#include "input/Input.h"

#include <stack>

namespace dm
{
	bool Input::GetKey(KeyCode keyCode)
	{
		auto window = static_cast<GLFWwindow *>(Application::get().get_window().get_glfw_window());
		auto state = glfwGetKey(window, underlying(keyCode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::GetMouseButton(MouseButton mouseButton)
	{
		auto window = static_cast<GLFWwindow *>(Application::get().get_window().get_glfw_window());
		auto state = glfwGetMouseButton(window, underlying(mouseButton));
		return state == GLFW_PRESS;
	}

	vec2f32 Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow *>(Application::get().get_window().get_glfw_window());
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		return {static_cast<f32>(mouseX), static_cast<f32>(mouseY)};
	}
}
