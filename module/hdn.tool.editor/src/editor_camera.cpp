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
		UpdateViewMatrix();
	}

	void EditorCamera::OnUpdate(Timestep ts, const UpdateState& updateState)
	{
		HDR_PROFILE_FUNCTION();

		if (updateState.LeftAltPressed)
		{
			vec2f32 delta = (updateState.MousePosition - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = updateState.MousePosition;

			if (updateState.MouseButtonMiddlePressed)
				Pan(delta);
			else if (updateState.MouseButtonLeftPressed)
				Rotate(delta);
			else if (updateState.MouseButtonRightPressed)
				Zoom(delta.y);
		}

		UpdateViewMatrix();
	}

	void EditorCamera::OnEvent(Event& event)
	{
		HDR_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(HDN_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	vec3f32 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), vec3f32(0.0f, 1.0f, 0.0f));
	}

	vec3f32 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), vec3f32(1.0f, 0.0f, 0.0f));
	}

	vec3f32 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), vec3f32(0.0f, 0.0f, -1.0f));
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(vec3f32(-m_Pitch, -m_Yaw, 0.0f));
	}

	void EditorCamera::UpdateProjectionMatrix()
	{
		HDR_PROFILE_FUNCTION();

		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateViewMatrix()
	{
		HDR_PROFILE_FUNCTION();

		// m_yaw = m_pitch = 0.0f; // Lock the camera rotation
		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(mat4f32(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		HDR_PROFILE_FUNCTION();

		f32 delta = e.GetOffsetY() * 0.1f;
		Zoom(delta);
		UpdateViewMatrix();
		return false;
	}

	void EditorCamera::Pan(const vec2f32& delta)
	{
		HDR_PROFILE_FUNCTION();

		auto [x_speed, y_speed] = GetPanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * x_speed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * y_speed * m_Distance;
	}

	void EditorCamera::Rotate(const vec2f32& delta)
	{
		HDR_PROFILE_FUNCTION();

		f32 yaw_sign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yaw_sign * delta.x * GetRotationSpeed(); // Y axis rotation
		m_Pitch += delta.y * GetRotationSpeed(); // X axis rotation
	}

	void EditorCamera::Zoom(f32 delta)
	{
		HDR_PROFILE_FUNCTION();

		m_Distance -= delta * GetZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	vec3f32 EditorCamera::CalculatePosition() const
	{
		HDR_PROFILE_FUNCTION();

		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	std::pair<f32, f32> EditorCamera::GetPanSpeed() const
	{
		HDR_PROFILE_FUNCTION();

		f32 x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		f32 xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		f32 y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		f32 yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	f32 EditorCamera::GetRotationSpeed() const
	{
		return 0.8f;
	}

	f32 EditorCamera::GetZoomSpeed() const
	{
		HDR_PROFILE_FUNCTION();

		f32 distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		f32 speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
	}

}