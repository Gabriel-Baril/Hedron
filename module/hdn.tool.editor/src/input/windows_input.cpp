#include "editor_application.h"
#include "input/Input.h"

#include <stack>

namespace hdn
{
	bool Input::GetKey(KeyCode keyCode)
	{
		auto window = static_cast<GLFWwindow*>(EditorApplication::Get().GetWindow().GetGLFWWindow());
		auto state = glfwGetKey(window, Underlying(keyCode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::GetMouseButton(MouseButton mouseButton)
	{
		auto window = static_cast<GLFWwindow*>(EditorApplication::Get().GetWindow().GetGLFWWindow());
		auto state = glfwGetMouseButton(window, Underlying(mouseButton));
		return state == GLFW_PRESS;
	}

	vec2f32 Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(EditorApplication::Get().GetWindow().GetGLFWWindow());
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		return { static_cast<f32>(mouseX), static_cast<f32>(mouseY) };
	}
}