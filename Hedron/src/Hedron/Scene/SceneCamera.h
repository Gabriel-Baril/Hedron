#pragma once

#include "Hedron/Renderer/Camera.h"

namespace Hedron
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void set_orthographic(float size, float nearClip, float farClip);
		void set_viewport_size(uint32_t width, uint32_t height);

		float get_orthographic_size() const { return m_orthographicsSize; };
		void  set_orthographic_size(float size)
		{ 
			m_orthographicsSize = size;
			recalculate_projection();
		};
	private:
		void recalculate_projection();
	private:
		float m_orthographicsSize = 10.0f;
		float m_orthographicsNear = -1.0f;
		float m_orthographicsFar = 1.0f;

		float m_aspectRatio = 0.0f;
	};
}