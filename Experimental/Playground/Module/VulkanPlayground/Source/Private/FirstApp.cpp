#include "VulkanPlayground/FirstApp.h"

namespace hdn
{
	FirstApp::FirstApp()
		: m_Window{ WIDTH, HEIGHT, "First App" }
	{
	}

	void FirstApp::Run()
	{
		while (!m_Window.ShouldClose())
		{
			glfwPollEvents();
		}
	}
}
