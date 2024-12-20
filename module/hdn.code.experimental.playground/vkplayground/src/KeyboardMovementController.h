#pragma once

#include "HDNGameObject.h"
#include "HDNWindow.h"

namespace hdn
{
	class KeyboardMovementController
	{
	public:
		struct KeyMappings {
			int moveLeft = GLFW_KEY_A;
			int moveRight = GLFW_KEY_D;
			int moveForward = GLFW_KEY_W;
			int moveBackward = GLFW_KEY_S;
			int moveUp = GLFW_KEY_E;
			int moveDown = GLFW_KEY_Q;
			int lookLeft = GLFW_KEY_LEFT;
			int lookRight = GLFW_KEY_RIGHT;
			int lookUp = GLFW_KEY_UP;
			int lookDown = GLFW_KEY_DOWN;
		};

		void MoveInPlaneXZ(GLFWwindow* window, float32 dt, HDNGameObject& gameObject);
	private:
		KeyMappings m_Keys{};
		float32 m_MoveSpeed{ 3.0f };
		float32 m_LookSpeed{ 1.5f };
	};
}