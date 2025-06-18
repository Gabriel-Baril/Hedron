#include "editor_camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "core/event/key_code.h"
#include "core/event/mouse_button_code.h"
#include "core/event/event_dispatcher.h"

#include "core/profiler/profiler.h"

namespace hdn
{
	EditorCamera::EditorCamera(f32 fov, f32 aspect_ratio, f32 near_clip, f32 far_clip)
		: m_FOV(fov), m_AspectRatio(aspect_ratio), m_NearClip(near_clip), m_FarClip(far_clip)
	{
		update_view_matrix();
	}

	void EditorCamera::on_update(Timestep ts, const UpdateState& updateState)
	{
		HDR_PROFILE_FUNCTION();

		if (updateState.LeftAltPressed)
		{
			vec2f32 delta = (updateState.MousePosition - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = updateState.MousePosition;

			if (updateState.MouseButtonMiddlePressed)
				pan(delta);
			else if (updateState.MouseButtonLeftPressed)
				rotate(delta);
			else if (updateState.MouseButtonRightPressed)
				zoom(delta.y);
		}

		update_view_matrix();
	}

	void EditorCamera::on_event(Event& event)
	{
		HDR_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.dispatch<MouseScrolledEvent>(HDN_BIND_EVENT_FN(EditorCamera::on_mouse_scroll));
	}

	vec3f32 EditorCamera::get_up_direction() const
	{
		return glm::rotate(get_orientation(), vec3f32(0.0f, 1.0f, 0.0f));
	}

	vec3f32 EditorCamera::get_right_direction() const
	{
		return glm::rotate(get_orientation(), vec3f32(1.0f, 0.0f, 0.0f));
	}

	vec3f32 EditorCamera::get_forward_direction() const
	{
		return glm::rotate(get_orientation(), vec3f32(0.0f, 0.0f, -1.0f));
	}

	glm::quat EditorCamera::get_orientation() const
	{
		return glm::quat(vec3f32(-m_Pitch, -m_Yaw, 0.0f));
	}

	void EditorCamera::update_projection_matrix()
	{
		HDR_PROFILE_FUNCTION();

		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::update_view_matrix()
	{
		HDR_PROFILE_FUNCTION();

		// m_yaw = m_pitch = 0.0f; // Lock the camera rotation
		m_Position = calculate_position();

		glm::quat orientation = get_orientation();
		m_ViewMatrix = glm::translate(mat4f32(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	bool EditorCamera::on_mouse_scroll(MouseScrolledEvent& e)
	{
		HDR_PROFILE_FUNCTION();

		f32 delta = e.get_offset_y() * 0.1f;
		zoom(delta);
		update_view_matrix();
		return false;
	}

	void EditorCamera::pan(const vec2f32& delta)
	{
		HDR_PROFILE_FUNCTION();

		auto [x_speed, y_speed] = get_pan_speed();
		m_FocalPoint += -get_right_direction() * delta.x * x_speed * m_Distance;
		m_FocalPoint += get_up_direction() * delta.y * y_speed * m_Distance;
	}

	void EditorCamera::rotate(const vec2f32& delta)
	{
		HDR_PROFILE_FUNCTION();

		f32 yaw_sign = get_up_direction().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yaw_sign * delta.x * get_rotation_speed(); // Y axis rotation
		m_Pitch += delta.y * get_rotation_speed(); // X axis rotation
	}

	void EditorCamera::zoom(f32 delta)
	{
		HDR_PROFILE_FUNCTION();

		m_Distance -= delta * get_zoom_speed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += get_forward_direction();
			m_Distance = 1.0f;
		}
	}

	vec3f32 EditorCamera::calculate_position() const
	{
		HDR_PROFILE_FUNCTION();

		return m_FocalPoint - get_forward_direction() * m_Distance;
	}

	std::pair<f32, f32> EditorCamera::get_pan_speed() const
	{
		HDR_PROFILE_FUNCTION();

		f32 x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		f32 xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		f32 y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		f32 yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	f32 EditorCamera::get_rotation_speed() const
	{
		return 0.8f;
	}

	f32 EditorCamera::get_zoom_speed() const
	{
		HDR_PROFILE_FUNCTION();

		f32 distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		f32 speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
	}

}