#pragma once

#include "Hedron/Renderer/Camera.h"

namespace Hedron
{
	// TODO: Put getters in CPP file
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType : uint32_t { PERSPECTIVE = 0, ORTHOGRAPHIC = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void set_orthographic(float size, float nearClip, float farClip);
		void set_perspective(float verticalFOV, float nearClip, float farClip);

		void set_viewport_size(uint32_t width, uint32_t height);

		// Perspective
		float get_perspective_vertical_fov() const { return m_perspectiveFOV; };
		void  set_perspective_vertical_fov(float verticalFOV)
		{
			m_perspectiveFOV = verticalFOV;
			recalculate_projection();
		};

		float get_perspective_far_clip() const { return m_perspectiveFar; }
		void set_perspective_far_clip(float farClip)
		{
			m_perspectiveFar = farClip;
			recalculate_projection();
		}

		float get_perspective_near_clip() const { return m_perspectiveNear; }
		void set_perspective_near_clip(float nearClip)
		{
			m_perspectiveNear = nearClip;
			recalculate_projection();
		}

		// Orthographic
		float get_orthographic_size() const { return m_orthographicSize; };
		void  set_orthographic_size(float size)
		{ 
			m_orthographicSize = size;
			recalculate_projection();
		};

		float get_orthographic_far_clip() const { return m_orthographicFar; }
		void set_orthographic_far_clip(float farClip) 
		{ 
			m_orthographicFar = farClip;
			recalculate_projection();
		}

		float get_orthographic_near_clip() const { return m_orthographicNear; }
		void set_orthographic_near_clip(float nearClip)
		{
			m_orthographicNear = nearClip;
			recalculate_projection();
		}

		ProjectionType get_projection_type() const { return m_projectionType; }
		void set_projection_type(uint32_t type)
		{ 
			m_projectionType = (ProjectionType)type;
			recalculate_projection();
		}


	private:
		void recalculate_projection();
	private:
		ProjectionType m_projectionType = ProjectionType::ORTHOGRAPHIC;

		float m_perspectiveFOV = glm::radians(45.0f);
		float m_perspectiveNear = 0.01f;
		float m_perspectiveFar = 1000.0f;

		float m_orthographicSize = 10.0f;
		float m_orthographicNear = -1.0f;
		float m_orthographicFar = 1.0f;

		float m_aspectRatio = 0.0f;
	};
}