#pragma once

#include "Hedron/Core/Timestep.h"

#include "Hedron/Renderer/OrthographicCamera.h"

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

	struct OrthographicCameraBounds
	{
		float left;
		float right;
		float bottom;
		float top;

		float get_width() const { return right - left; }
		float get_height() const { return top - bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false); // aspectratio * 2 units

		void on_update(Timestep ts);
		void on_event(Event& e);

		OrthographicCamera& get_camera() { return m_camera; }
		const OrthographicCamera& get_camera() const { return m_camera; }

		float get_aspect_ratio() { return m_aspectRatio; }
		float get_zoom_level() const { return m_zoomLevel; }
		const OrthographicCameraBounds& get_bounds() const { return m_bounds; }


		void set_zoom_level(float zoomLevel)
		{
			m_zoomLevel = zoomLevel;
			calculate_view();
		}

	private:
		void calculate_view();

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

		OrthographicCameraBounds m_bounds;
		OrthographicCamera m_camera;

		MoveKeyCallback m_moveKeyCallback;
	};
}