#include "VulkanPlayground/FirstApp.h"

#include "VulkanPlayground/KeyboardMovementController.h"
#include "VulkanPlayground/HDNCamera.h"
#include "VulkanPlayground/SimpleRenderSystem.h"

#include "Core/Core.h"
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <chrono>

namespace hdn
{
	static constexpr float32 MAX_FRAME_TIME = 0.5f;

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
		HDNCamera camera{};
		camera.SetViewDirection(vec3f32{ 0.0f }, vec3f32{0.5f, 0.0f, 1.0f});

		auto viewerObject = HDNGameObject::CreateGameObject();
		KeyboardMovementController cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!m_Window.ShouldClose())
		{
			glfwPollEvents();
			
			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float32, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			frameTime = glm::min(frameTime, MAX_FRAME_TIME);

			cameraController.MoveInPlaneXZ(m_Window.GetGLFWWindow(), frameTime, viewerObject);
			camera.SetViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			float32 aspect = m_Renderer.GetAspectRatio();
			camera.SetPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 1.0f);

			if (auto commandBuffer = m_Renderer.BeginFrame())
			{
				
				// Begin offscreen shadow pass
				// Render shadow casting objects
				// end offscreen shadow pass

				m_Renderer.BeginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.RenderGameObjects(commandBuffer, m_GameObjects, camera);
				m_Renderer.EndSwapChainRenderPass(commandBuffer);

				m_Renderer.EndFrame();
			}
		}

		vkDeviceWaitIdle(m_Device.device());
	}

	void FirstApp::LoadGameObjects()
	{
		Ref<HDNModel> hdnModel = HDNModel::CreateModelFromFile(&m_Device, "Models/smooth_vase.obj");
		auto gameObj = HDNGameObject::CreateGameObject();
		gameObj.model = hdnModel;
		gameObj.transform.translation = { 0.0f, 0.0f, 2.5f };
		gameObj.transform.scale = vec3f32{3.0f};
		m_GameObjects.push_back(std::move(gameObj));
	}
}
