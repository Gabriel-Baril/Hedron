#include "VulkanPlayground/KeyboardMovementController.h"

#include <limits>

namespace hdn
{
	void KeyboardMovementController::MoveInPlaneXZ(GLFWwindow* window, float32 dt, HDNGameObject& gameObject)
	{
		vec3f32 rotate{0.0f};
		if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.0f;
		if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.0f;
		if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.0f;
		if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.0f;

		if (glm::dot(rotate, rotate) > std::numeric_limits<float32>::epsilon())
		{
			gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
		}

		gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f); // Prevents the object to go upside down
		gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float32>());

		float32 yaw = gameObject.transform.rotation.y;
		const vec3f32 forwardDirection{ sin(yaw), 0.0f, cos(yaw) };
		const vec3f32 rightDirection{ forwardDirection.z, 0.0f, -forwardDirection.x };
		const vec3f32 upDirection{ 0.0f, -1.0f, 0.0f };

		vec3f32 moveDirection{ 0 };
		if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDirection += forwardDirection;
		if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDirection -= forwardDirection;
		if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDirection += rightDirection;
		if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDirection -= rightDirection;
		if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDirection += upDirection;
		if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDirection -= upDirection;

		if (glm::dot(moveDirection, moveDirection) > std::numeric_limits<float32>::epsilon())
		{
			gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDirection);
		}
	}
}
