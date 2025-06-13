#include "keyboard_movement_controller.h"

#include <limits>

#include "ecs/components/transform_component.h"

namespace hdn
{
	void KeyboardMovementController::MoveInPlaneXZ(GLFWwindow* window, f32 dt, HDNGameObject& gameObject)
	{
		vec3f32 rotate{0.0f};
		if (glfwGetKey(window, m_Keys.lookRight) == GLFW_PRESS) rotate.y += 1.0f;
		if (glfwGetKey(window, m_Keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.0f;
		if (glfwGetKey(window, m_Keys.lookUp) == GLFW_PRESS) rotate.x += 1.0f;
		if (glfwGetKey(window, m_Keys.lookDown) == GLFW_PRESS) rotate.x -= 1.0f;

		TransformComponent* transformC = gameObject.GetMut<TransformComponent>();

		if (glm::dot(rotate, rotate) > std::numeric_limits<f32>::epsilon())
		{
			transformC->rotation += m_LookSpeed * dt * glm::normalize(rotate);
		}

		transformC->rotation.x = glm::clamp(transformC->rotation.x, -1.5f, 1.5f); // Prevents the object to go upside down
		transformC->rotation.y = glm::mod(transformC->rotation.y, glm::two_pi<f32>());

		f32 yaw = transformC->rotation.y;
		const vec3f32 forwardDirection{ sin(yaw), 0.0f, cos(yaw) };
		const vec3f32 rightDirection{ forwardDirection.z, 0.0f, -forwardDirection.x };
		const vec3f32 upDirection{ 0.0f, -1.0f, 0.0f };

		vec3f32 moveDirection{ 0 };
		if (glfwGetKey(window, m_Keys.moveForward) == GLFW_PRESS) moveDirection += forwardDirection;
		if (glfwGetKey(window, m_Keys.moveBackward) == GLFW_PRESS) moveDirection -= forwardDirection;
		if (glfwGetKey(window, m_Keys.moveRight) == GLFW_PRESS) moveDirection += rightDirection;
		if (glfwGetKey(window, m_Keys.moveLeft) == GLFW_PRESS) moveDirection -= rightDirection;
		if (glfwGetKey(window, m_Keys.moveUp) == GLFW_PRESS) moveDirection += upDirection;
		if (glfwGetKey(window, m_Keys.moveDown) == GLFW_PRESS) moveDirection -= upDirection;

		if (glm::dot(moveDirection, moveDirection) > std::numeric_limits<f32>::epsilon())
		{
			transformC->position += m_MoveSpeed * dt * glm::normalize(moveDirection);
		}
	}
}
