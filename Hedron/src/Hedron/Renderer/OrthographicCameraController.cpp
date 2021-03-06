#include "hdrpch.h"

#include "Hedron/Core/Input.h"
#include "Hedron/Core/KeyCodes.h"

#include "Hedron/Renderer/OrthographicCameraController.h"

namespace Hedron
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_aspectRatio(aspectRatio), m_rotation(rotation),
		m_bounds{ -m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel },
		m_camera(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top)
	{
	}

	void OrthographicCameraController::on_update(Timestep ts)
	{
		HDR_PROFILE_FUNCTION();
		if (Input::is_key_pressed(Hedron::KeyCode::W))
			m_cameraPosition.y += m_cameraTranslationSpeed * ts;
		else if (Input::is_key_pressed(Hedron::KeyCode::S))
			m_cameraPosition.y -= m_cameraTranslationSpeed * ts;

		if (Input::is_key_pressed(Hedron::KeyCode::A))
			m_cameraPosition.x -= m_cameraTranslationSpeed * ts;
		else if (Input::is_key_pressed(Hedron::KeyCode::D))
			m_cameraPosition.x += m_cameraTranslationSpeed * ts;

		if (m_rotation)
		{
			if (Input::is_key_pressed(Hedron::KeyCode::Q))
				m_cameraRotation += m_cameraRotationSpeed * ts;
			else if (Input::is_key_pressed(Hedron::KeyCode::E))
				m_cameraRotation -= m_cameraRotationSpeed * ts;

			m_camera.set_rotation(m_cameraRotation);
		}

		m_camera.set_position(m_cameraPosition);
	}

	void OrthographicCameraController::on_event(Event& e)
	{
		HDR_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(HDR_BIND_EVENT_FN(OrthographicCameraController::on_mouse_scrolled_event));
		dispatcher.dispatch<WindowResizeEvent>(HDR_BIND_EVENT_FN(OrthographicCameraController::on_window_resize_event));
	}

	void OrthographicCameraController::on_resize(float width, float height)
	{
		m_aspectRatio = width / height;
		this->calculate_view();
	}

	void OrthographicCameraController::calculate_view()
	{
		m_bounds = { -m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel };
		m_camera.set_projection(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top);
	}

	bool OrthographicCameraController::on_mouse_scrolled_event(MouseScrolledEvent& e)
	{
		HDR_PROFILE_FUNCTION();

		m_zoomLevel -= e.get_y_offset() / 5.0f;
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);
		m_cameraTranslationSpeed = m_zoomLevel;
		calculate_view();
		return false;
	}

	bool OrthographicCameraController::on_window_resize_event(WindowResizeEvent& e)
	{
		HDR_PROFILE_FUNCTION();
		on_resize((float)e.get_width(), (float)e.get_height());
		return false;
	}

}