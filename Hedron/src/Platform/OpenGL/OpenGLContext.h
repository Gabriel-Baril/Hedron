#pragma once
#include "Hedron/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Hedron
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void init() override;
		virtual void swap_buffers() override;
	private:
		GLFWwindow* m_windowHandle;
	};
}