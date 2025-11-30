#include "keyboard_movement_controller.h"

#include <limits>

#include "hobj/scene/transform_component.h"
#include "input/input.h"

namespace dm
{
	void KeyboardMovementController::move_in_plane_xyz(GLFWwindow *window, f32 dt, Entity &gameObject)
	{
		vec3f32 rotate{0.0f};
		if (Input::GetKey(m_Keys.lookRight))
			rotate.y += 1.0f;
		if (Input::GetKey(m_Keys.lookLeft))
			rotate.y -= 1.0f;
		if (Input::GetKey(m_Keys.lookUp))
			rotate.x += 1.0f;
		if (Input::GetKey(m_Keys.lookDown))
			rotate.x -= 1.0f;

		TransformComponent *transformC = gameObject.get_mut<TransformComponent>();

		if (glm::dot(rotate, rotate) > std::numeric_limits<f32>::epsilon())
		{
			transformC->rotation += m_LookSpeed * dt * glm::normalize(rotate);
		}

		transformC->rotation.x = glm::clamp(transformC->rotation.x, -1.5f, 1.5f); // Prevents the object to go upside down
		transformC->rotation.y = glm::mod(transformC->rotation.y, glm::two_pi<f32>());

		f32 yaw = transformC->rotation.y;
		const vec3f32 forwardDirection{sin(yaw), 0.0f, cos(yaw)};
		const vec3f32 rightDirection{forwardDirection.z, 0.0f, -forwardDirection.x};
		const vec3f32 upDirection{0.0f, -1.0f, 0.0f};

		vec3f32 moveDirection{0};
		if (Input::GetKey(m_Keys.moveForward))
			moveDirection += forwardDirection;
		if (Input::GetKey(m_Keys.moveBackward))
			moveDirection -= forwardDirection;
		if (Input::GetKey(m_Keys.moveRight))
			moveDirection += rightDirection;
		if (Input::GetKey(m_Keys.moveLeft))
			moveDirection -= rightDirection;
		if (Input::GetKey(m_Keys.moveUp))
			moveDirection += upDirection;
		if (Input::GetKey(m_Keys.moveDown))
			moveDirection -= upDirection;

		if (glm::dot(moveDirection, moveDirection) > std::numeric_limits<f32>::epsilon())
		{
			transformC->position += m_MoveSpeed * dt * glm::normalize(moveDirection);
		}
	}
}
