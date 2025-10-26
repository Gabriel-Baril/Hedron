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

		void on_update(Timestep ts, const UpdateState& keyState);
		void on_event(Event& event);

		inline f32 get_distance() const { return m_Distance; }
		inline void set_distance(f32 distance) { m_Distance = distance; }

		inline void set_viewport_size(f32 width, f32 height)
		{
			m_ViewportWidth = width;
			m_ViewportHeight = height;
			update_projection_matrix();
		};

		const mat4f32& get_view_matrix() const { return m_ViewMatrix; }
		const mat4f32& get_inverse_view_matrix() const { return glm::inverse(m_ViewMatrix); }
		mat4f32 get_view_projection_matrix() const { return m_Projection * m_ViewMatrix; }

		vec3f32 get_up_direction() const;
		vec3f32 get_right_direction() const;
		vec3f32 get_forward_direction() const;
		const vec3f32& get_position() const { return m_Position; }
		glm::quat get_orientation() const;

		f32 get_pitch() const { return m_Pitch; }
		f32 get_yaw() const { return m_Yaw; }
	private:
		void update_projection_matrix();
		void update_view_matrix();

		bool on_mouse_scroll(MouseScrolledEvent& e);

		void pan(const vec2f32& delta);
		void rotate(const vec2f32& delta);
		void zoom(f32 delta);

		vec3f32 calculate_position() const;;

		std::pair<f32, f32> get_pan_speed() const;
		f32 get_rotation_speed() const;
		f32 get_zoom_speed() const;
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