#pragma once

#include <glm/glm.hpp>

#include "Hedron/Renderer/Camera.h"
#include "Hedron/Core/Timestep.h"
#include "Hedron/Events/Event.h"
#include "Hedron/Events/MouseEvents.h"


namespace Hedron
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float FOV, float aspectRatio, float nearClip, float farClip);

		void on_update(Timestep ts);
		void on_event(Event& event);

		inline float get_distance() const { return m_distance; }
		inline void set_distance(float distance) { m_distance = distance; }

		inline void set_viewport_size(float width, float height) 
		{
			m_viewportWidth = width;
			m_viewportHeight = height;
			update_projection();
		};

		const glm::mat4& get_view_matrix() const { return m_viewMatrix; }
		glm::mat4 get_view_projection() const { return m_projection * m_viewMatrix; }

		glm::vec3 get_up_direction() const;
		glm::vec3 get_right_direction() const;
		glm::vec3 get_forward_direction() const;
		const glm::vec3& get_position() const { return m_position; }
		glm::quat get_orientation() const;

		float get_pitch() const { return m_pitch; }
		float get_yaw() const { return m_yaw; }
	private:
		void update_projection();
		void update_view();

		bool on_mouse_scroll(MouseScrolledEvent& e);

		void mouse_pan(const glm::vec2& delta);
		void mouse_rotate(const glm::vec2& delta);
		void mouse_zoom(float delta);

		glm::vec3 calculate_position() const;;

		std::pair<float, float> pan_speed() const;
		float rotation_speed() const;
		float zoom_speed() const;
	private:
		float m_FOV = 45.0f;
		float m_aspectRatio = 1.778f;
		float m_nearClip = 0.1f;
		float m_farClip = 1000.0f;

		glm::mat4 m_viewMatrix;
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_focalPoint = { 0.0f, 0.0f, 0.0f };
	
		glm::vec2 m_initialMousePosition;

		float m_distance = 10.0f;
		float m_pitch = 0.0f;
		float m_yaw = 0.0f;

		float m_viewportWidth = 1280;
		float m_viewportHeight = 720;
	};
}