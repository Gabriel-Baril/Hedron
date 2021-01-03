#include "hdrpch.h"

#include "Hedron/Core/Input.h"
#include "Hedron/Core/KeyCodes.h"

#include "Hedron/Renderer/OrthographicCameraController.h"

namespace Hedron
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_aspectRatio(aspectRatio), m_rotation(rotation),
		m_orthoCamera(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel)
	{

	}

	void OrthographicCameraController::on_update(Timestep ts)
	{
		if (Input::is_key_pressed(HDR_KEY_W))
			m_cameraPosition.y += m_cameraTranslationSpeed * ts;
		else if (Input::is_key_pressed(HDR_KEY_S))
			m_cameraPosition.y -= m_cameraTranslationSpeed * ts;

		if (Input::is_key_pressed(HDR_KEY_A))
			m_cameraPosition.x -= m_cameraTranslationSpeed * ts;
		else if (Input::is_key_pressed(HDR_KEY_D))
			m_cameraPosition.x += m_cameraTranslationSpeed * ts;

		if (m_rotation)
		{
			if (Input::is_key_pressed(HDR_KEY_Q))
				m_cameraRotation += m_cameraRotationSpeed * ts;
			else if (Input::is_key_pressed(HDR_KEY_E))
				m_cameraRotation -= m_cameraRotationSpeed * ts;

			m_orthoCamera.set_rotation(m_cameraRotation);
		}

		m_orthoCamera.set_position(m_cameraPosition);
	}

	void OrthographicCameraController::on_event(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(HDR_BIND_EVENT_FN(OrthographicCameraController::on_mouse_scrolled_event));
		dispatcher.dispatch<WindowResizeEvent>(HDR_BIND_EVENT_FN(OrthographicCameraController::on_window_resize_event));
	}

	bool OrthographicCameraController::on_mouse_scrolled_event(MouseScrolledEvent& e)
	{
		m_zoomLevel -= e.get_y_offset() / 5.0f;
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);
		m_cameraTranslationSpeed = m_zoomLevel;
		m_orthoCamera.set_projection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		return false;
	}

	bool OrthographicCameraController::on_window_resize_event(WindowResizeEvent& e)
	{
		m_aspectRatio = (float)e.get_width() / (float)e.get_height();
		m_orthoCamera.set_projection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		return false;
	}

}