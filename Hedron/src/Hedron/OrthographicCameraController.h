#pragma once

#include "Hedron/Renderer/OrthographicCamera.h"

#include "Hedron/Core/Timestep.h"
#include "Hedron/Events/ApplicationEvents.h"
#include "Hedron/Events/MouseEvents.h"
#include "Hedron/Events/KeyboardEvents.h"


namespace Hedron
{
	struct MoveKeyCallback
	{
		uint32_t moveUpKey;
		uint32_t moveDownKey;
		uint32_t moveRightKey;
		uint32_t moveLeftKey;
		uint32_t zoomOut;
		uint32_t zoomIn;
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false); // aspectratio * 2 units

		void on_update(Timestep ts);
		void on_event(Event& e);

		OrthographicCamera& get_camera() { return m_orthoCamera; }

	private:
		bool on_mouse_scrolled_event(MouseScrolledEvent& e);
		bool on_window_resize_event(WindowResizeEvent& e);
	private:
		float m_aspectRatio;
		float m_zoomLevel = 1;

		glm::vec3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_cameraTranslationSpeed = 5.0f;

		bool m_rotation;
		float m_cameraRotation = 0.0f;
		float m_cameraRotationSpeed = 180.0f;

		OrthographicCamera m_orthoCamera;
		MoveKeyCallback m_moveKeyCallback;
	};
}