#pragma once

#include "gameobject.h"
#include "gfx/r_vk_window.h"
#include "core/event/key_code.h"

namespace dm
{
	class KeyboardMovementController
	{
	public:
		struct KeyMappings
		{
			KeyCode moveLeft = KeyCode::A;
			KeyCode moveRight = KeyCode::D;
			KeyCode moveForward = KeyCode::W;
			KeyCode moveBackward = KeyCode::S;
			KeyCode moveUp = KeyCode::E;
			KeyCode moveDown = KeyCode::Q;
			KeyCode lookLeft = KeyCode::Left;
			KeyCode lookRight = KeyCode::Right;
			KeyCode lookUp = KeyCode::Up;
			KeyCode lookDown = KeyCode::Down;
		};

		void move_in_plane_xyz(GLFWwindow *window, f32 dt, Entity &gameObject);

	private:
		KeyMappings m_Keys{};
		f32 m_MoveSpeed{3.0f};
		f32 m_LookSpeed{1.5f};
	};
}
