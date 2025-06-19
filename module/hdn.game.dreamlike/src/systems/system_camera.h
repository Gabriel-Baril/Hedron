#pragma once

#include "core/core.h"
#include "frame_info.h"

#include "core/application/system.h"
#include "keyboard_movement_controller.h"
#include "gameobject.h"

namespace hdn
{
	inline static constexpr const char* NAME_CAMERA_SYSTEM = "CameraSystem";

	class CameraSystem : public ISystem
	{
	public:
		CameraSystem()
		{
		}

		void init();
		void update(f32 frameTime);
		vec3f32 get_camera_position() { return camera.get_position(); };
		const HDNCamera& get_camera() { return camera; }
	private:
		HDNCamera camera{};
		KeyboardMovementController cameraController{};
		Entity m_ViewerObject;
	};
}