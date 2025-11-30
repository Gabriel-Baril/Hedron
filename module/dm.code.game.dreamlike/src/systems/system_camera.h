#pragma once

#include "core/core.h"
#include "frame_info.h"

#include "core/application/system.h"
#include "keyboard_movement_controller.h"
#include "gameobject.h"

namespace dm
{
	inline static constexpr const char *NAME_CAMERA_SYSTEM = "CameraSystem";

	class CameraSystem : public ISystem
	{
	public:
		CameraSystem()
		{
		}

		void init();
		void update(f32 frameTime);
		vec3f32 get_camera_position() { return camera.get_position(); };
		const DMCamera &get_camera() { return camera; }

	private:
		DMCamera camera{};
		KeyboardMovementController cameraController{};
		Entity m_ViewerObject;
	};
}
