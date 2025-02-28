#pragma once

#include <glm/glm.hpp>

#include "core/core.h"
#include "camera.h"
#include "core/Timestep.h"
#include "core/event/Event.h"
#include "core/event/mouse_scrolled_event.h"


namespace hdn
{
	class EditorCamera : public Camera
	{
	public:
		struct UpdateState
		{
			bool LeftAltPressed;
			bool MouseButtonMiddlePressed;
			bool MouseButtonLeftPressed;
			bool MouseButtonRightPressed;
			vec2f32 MousePosition;
		};


		EditorCamera() = default;
		EditorCamera(f32 FOV, f32 aspectRatio, f32 nearClip, f32 farClip);

		void OnUpdate(Timestep ts, const UpdateState& keyState);
		void OnEvent(Event& event);

		inline f32 GetDistance() const { return m_Distance; }
		inline void SetDistance(f32 distance) { m_Distance = distance; }

		inline void SetViewportSize(f32 width, f32 height)
		{
			m_ViewportWidth = width;
			m_ViewportHeight = height;
			UpdateProjectionMatrix();
		};

		const mat4f32& GetViewMatrix() const { return m_ViewMatrix; }
		const mat4f32& GetInverseViewMatrix() const { return glm::inverse(m_ViewMatrix); }
		mat4f32 GetViewProjectionMatrix() const { return m_Projection * m_ViewMatrix; }

		vec3f32 GetUpDirection() const;
		vec3f32 GetRightDirection() const;
		vec3f32 GetForwardDirection() const;
		const vec3f32& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		f32 GetPitch() const { return m_Pitch; }
		f32 GetYaw() const { return m_Yaw; }
	private:
		void UpdateProjectionMatrix();
		void UpdateViewMatrix();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void Pan(const vec2f32& delta);
		void Rotate(const vec2f32& delta);
		void Zoom(f32 delta);

		vec3f32 CalculatePosition() const;;

		std::pair<f32, f32> GetPanSpeed() const;
		f32 GetRotationSpeed() const;
		f32 GetZoomSpeed() const;
	private:
		f32 m_FOV = 45.0f;
		f32 m_AspectRatio = 1.778f;
		f32 m_NearClip = 0.1f;
		f32 m_FarClip = 1000.0f;

		mat4f32 m_ViewMatrix;
		vec3f32 m_Position = { 0.0f, 0.0f, 0.0f };
		vec3f32 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		vec2f32 m_InitialMousePosition;

		f32 m_Distance = 10.0f;
		f32 m_Pitch = 0.0f;
		f32 m_Yaw = 0.0f;

		f32 m_ViewportWidth = 800;
		f32 m_ViewportHeight = 600;
	};
}