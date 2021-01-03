#include "hdrpch.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLContext.h"

namespace Hedron
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
		HDR_CORE_ASSERT(windowHandle, "Window handle is null");
	}

	void OpenGLContext::init()
	{
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HDR_CORE_ASSERT(status, "Failed to initialize GLAD");

		HDR_CORE_INFO("OpenGL Info :");
		HDR_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		HDR_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		HDR_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::swap_buffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}
}

