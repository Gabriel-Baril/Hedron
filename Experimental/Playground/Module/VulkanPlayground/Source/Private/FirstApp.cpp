#include "VulkanPlayground/FirstApp.h"

#include "VulkanPlayground/SimpleRenderSystem.h"

#include "Core/Core.h"
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

namespace hdn
{
	FirstApp::FirstApp()
	{
		LoadGameObjects();
	}

	FirstApp::~FirstApp()
	{
	}

	void FirstApp::Run()
	{
		SimpleRenderSystem simpleRenderSystem{ &m_Device, m_Renderer.GetSwapChainRenderPass() };

		while (!m_Window.ShouldClose())
		{
			glfwPollEvents();
			if (auto commandBuffer = m_Renderer.BeginFrame())
			{
				// Begin offscreen shadow pass
				// Render shadow casting objects
				// end offscreen shadow pass

				m_Renderer.BeginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.RenderGameObjects(commandBuffer, m_GameObjects);
				m_Renderer.EndSwapChainRenderPass(commandBuffer);

				m_Renderer.EndFrame();
			}
		}

		vkDeviceWaitIdle(m_Device.device());
	}

	void FirstApp::LoadGameObjects()
	{
		std::vector<HDNModel::Vertex> vertices{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};
		auto hdnModel = CreateRef<HDNModel>(&m_Device, vertices);
		auto triangle = HDNGameObject::CreateGameObject();
		triangle.model = hdnModel;
		triangle.color = { 0.0f, 0.8f, 0.1f };
		triangle.transform2d.translation.x = 0.2f;
		triangle.transform2d.scale = { 2.0f, 0.5f };
		triangle.transform2d.rotation = .25f * glm::two_pi<float32>();
		m_GameObjects.push_back(std::move(triangle));
	}
}
