#include "hdrpch.h"
#include "EditorCamera.h"

#include "Hedron/Core/Input.h"
#include "Hedron/Core/KeyCodes.h"
#include "Hedron/Core/MouseButtonCodes.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Hedron
{

	EditorCamera::EditorCamera(float FOV, float aspectRatio, float nearClip, float farClip)
		: m_FOV(FOV), m_aspectRatio(aspectRatio), m_nearClip(nearClip), m_farClip(farClip)
	{
		update_view();
	}

	void EditorCamera::on_update(Timestep ts)
	{
		if (Input::is_key_pressed(KeyCode::LEFT_ALT))
		{
			const glm::vec2& mouse{ Input::get_mouse_x(), Input::get_mouse_y() };
			glm::vec2 delta = (mouse - m_initialMousePosition) * 0.003f;
			m_initialMousePosition = mouse;

			if (Input::is_mouse_button_pressed(MouseButton::MOUSE_BUTTON_MIDDLE))
				mouse_pan(delta);
			else if (Input::is_mouse_button_pressed(MouseButton::MOUSE_BUTTON_LEFT))
				mouse_rotate(delta);
			else if (Input::is_mouse_button_pressed(MouseButton::MOUSE_BUTTON_RIGHT))
				mouse_zoom(delta.y);
		}

		update_view();
	}

	void EditorCamera::on_event(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<MouseScrolledEvent>(HDR_BIND_EVENT_FN(EditorCamera::on_mouse_scroll));
	}

	glm::vec3 EditorCamera::get_up_direction() const
	{
		return glm::rotate(get_orientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::get_right_direction() const
	{
		return glm::rotate(get_orientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::get_forward_direction() const
	{
		return glm::rotate(get_orientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat EditorCamera::get_orientation() const
	{
		return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.0f));
	}

	void EditorCamera::update_projection()
	{
		m_aspectRatio = m_viewportWidth / m_viewportHeight;
		m_projection = glm::perspective(glm::radians(m_FOV), m_aspectRatio, m_nearClip, m_farClip);
	}

	void EditorCamera::update_view()
	{
		// m_yaw = m_pitch = 0.0f; // Lock the camera rotation
		m_position = calculate_position();

		glm::quat orientation = get_orientation();
		m_viewMatrix = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(orientation);
		m_viewMatrix = glm::inverse(m_viewMatrix);
	}

	bool EditorCamera::on_mouse_scroll(MouseScrolledEvent& e)
	{
		float delta = e.get_y_offset() * 0.1f;
		mouse_zoom(delta);
		update_view();
		return false;
	}

	void EditorCamera::mouse_pan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = pan_speed();
		m_focalPoint += -get_right_direction() * delta.x * xSpeed * m_distance;
		m_focalPoint += get_up_direction() * delta.y * ySpeed * m_distance;
	}

	void EditorCamera::mouse_rotate(const glm::vec2& delta)
	{
		float yawSign = get_up_direction().y < 0 ? -1.0f : 1.0f;
		m_yaw += yawSign * delta.x * rotation_speed(); // Y axis rotation
		m_pitch += delta.y * rotation_speed(); // X axis rotation
	}

	void EditorCamera::mouse_zoom(float delta)
	{
		m_distance -= delta * zoom_speed();
		if (m_distance < 1.0f)
		{
			m_focalPoint += get_forward_direction();
			m_distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::calculate_position() const
	{
		return m_focalPoint - get_forward_direction() * m_distance;
	}

	std::pair<float, float> EditorCamera::pan_speed() const
	{
		float x = std::min(m_viewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_viewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::rotation_speed() const
	{
		return 0.8f;
	}

	float EditorCamera::zoom_speed() const
	{
		float distance = m_distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
	}

}