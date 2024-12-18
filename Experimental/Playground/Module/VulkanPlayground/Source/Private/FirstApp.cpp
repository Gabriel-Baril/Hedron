#include "VulkanPlayground/FirstApp.h"

#include "VulkanPlayground/KeyboardMovementController.h"
#include "VulkanPlayground/HDNBuffer.h"
#include "VulkanPlayground/HDNCamera.h"

#include "VulkanPlayground/SimpleRenderSystem.h"
#include "VulkanPlayground/PointLightSystem.h"
#include "VulkanPlayground/HDNImgui.h"

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
		globalPool = HDNDescriptorPool::Builder(m_Device)
			.setMaxSets(HDNSwapChain::MAX_FRAMES_IN_FLIGHT) // The maximum amount of sets in the pools
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, HDNSwapChain::MAX_FRAMES_IN_FLIGHT) // The number of uniform descriptor in the descriptor pool
			.build();
		
		LoadGameObjects();
	}

	FirstApp::~FirstApp()
	{
	}

	void FirstApp::Run()
	{
		std::vector<Scope<HDNBuffer>> uboBuffers(HDNSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0;i < uboBuffers.size(); i++)
		{
			uboBuffers[i] = CreateScope<HDNBuffer>(
				&m_Device,
				sizeof(GlobalUbo),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);
			uboBuffers[i]->map();
		}

		auto globalSetLayout = HDNDescriptorSetLayout::Builder(m_Device)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
			.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(HDNSwapChain::MAX_FRAMES_IN_FLIGHT); // One descriptor set per frame
		for (int i = 0;i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			HDNDescriptorWriter(*globalSetLayout, *globalPool)
				.writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}

		SimpleRenderSystem simpleRenderSystem{ &m_Device, m_Renderer.GetSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };
		PointLightSystem pointLightSystem{ &m_Device, m_Renderer.GetSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };
		HDNCamera camera{};

		auto viewerObject = HDNGameObject::CreateGameObject();
		viewerObject.transform.translation.z = -2.5f;
		KeyboardMovementController cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		ImguiSystem imguiSystem;
		
		Scope<HDNDescriptorPool> imguiDescriptorPool = HDNDescriptorPool::Builder(m_Device)
			.setPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
			.setMaxSets(1)
			.addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)
			.build();
		
		QueueFamilyIndices queueFamilyIndices = m_Device.findPhysicalQueueFamilies();
		
		imguiSystem.Init(
			m_Window.GetGLFWWindow(),
			m_Device.surface(),
			m_Device.GetInstance(),
			m_Device.GetPhysicalDevice(),
			m_Device.device(),
			queueFamilyIndices.graphicsFamily,
			m_Device.graphicsQueue(),
			imguiDescriptorPool->GetDescriptor()
		);

		while (!m_Window.ShouldClose())
		{
			glfwPollEvents();
			
			imguiSystem.HandleWindowResize();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float32, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			frameTime = glm::min(frameTime, MAX_FRAME_TIME);

			cameraController.MoveInPlaneXZ(m_Window.GetGLFWWindow(), frameTime, viewerObject);
			camera.SetViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			float32 aspect = m_Renderer.GetAspectRatio();
			camera.SetPerspectiveProjection(glm::radians(50.0f), aspect, 0.01f, 1.0f);

			imguiSystem.NewFrame();

			ImGui::Begin("Hello, world!");
			ImGui::Text("This is some useful text.");
			ImGui::End();

			imguiSystem.Render(ImVec4(0.45f, 0.55f, 0.60f, 1.00f));

			if (auto commandBuffer = m_Renderer.BeginFrame())
			{
				int frameIndex = m_Renderer.GetFrameIndex();
				FrameInfo frameInfo{};
				frameInfo.frameIndex = frameIndex;
				frameInfo.frameTime = frameTime;
				frameInfo.commandBuffer = commandBuffer;
				frameInfo.camera = &camera;
				frameInfo.globalDescriptorSet = globalDescriptorSets[frameIndex];
				frameInfo.gameObjects = &m_GameObjects;

				// update
				GlobalUbo ubo{};
				ubo.projection = camera.GetProjection();
				ubo.view = camera.GetView();
				ubo.inverseView = camera.GetInverseView();

				pointLightSystem.Update(frameInfo, ubo);

				uboBuffers[frameIndex]->writeToBuffer((void*)&ubo);
				uboBuffers[frameIndex]->flush();

				// render
				m_Renderer.BeginSwapChainRenderPass(commandBuffer);

				// Order Here Matters
				simpleRenderSystem.RenderGameObjects(frameInfo);
				pointLightSystem.Render(frameInfo);

				m_Renderer.EndSwapChainRenderPass(commandBuffer);

				m_Renderer.EndFrame();
			}
		}

		vkDeviceWaitIdle(m_Device.device());
	}

	void FirstApp::LoadGameObjects()
	{
		Ref<HDNModel> hdnModel = HDNModel::CreateModelFromFile(&m_Device, "Models/flat_vase.obj");
		auto flatVase = HDNGameObject::CreateGameObject();
		flatVase.model = hdnModel;
		flatVase.transform.translation = { -0.5f, 0.5f, 0.0f };
		flatVase.transform.scale = vec3f32{3.0f, 1.5f, 3.0f};
		m_GameObjects.emplace(flatVase.GetID(), std::move(flatVase));

		hdnModel = HDNModel::CreateModelFromFile(&m_Device, "Models/smooth_vase.obj");
		auto smoothVase = HDNGameObject::CreateGameObject();
		smoothVase.model = hdnModel;
		smoothVase.transform.translation = { -1.0f, 0.5f, 0.0f };
		smoothVase.transform.scale = vec3f32{ 1.0f, 1.0f, 1.0f };
		m_GameObjects.emplace(smoothVase.GetID(), std::move(smoothVase));

		hdnModel = HDNModel::CreateModelFromFile(&m_Device, "Models/quad.obj");
		auto floor = HDNGameObject::CreateGameObject();
		floor.model = hdnModel;
		floor.transform.translation = { 0.0f, 0.5f, 0.0f };
		floor.transform.scale = vec3f32{ 3.0f, 1.0f, 3.0f };
		m_GameObjects.emplace(floor.GetID(), std::move(floor));

		std::vector<glm::vec3> lightColors{
			{1.f, .1f, .1f},
			{.1f, .1f, 1.f},
			{.1f, 1.f, .1f},
			{1.f, 1.f, .1f},
			{.1f, 1.f, 1.f},
			{1.f, 1.f, 1.f} 
		};

		for (int i = 0;i < lightColors.size(); i++)
		{
			auto pointLight = HDNGameObject::MakePointLight(0.2f);
			pointLight.color = lightColors[i];
			auto rotateLight = glm::rotate(mat4f32(1.0f), (i * glm::two_pi<float32>()) / lightColors.size(), {0.0f, -1.0f, 0.0f});
			pointLight.transform.translation = vec3f32(rotateLight * vec4f32(-1.0f, -1.0f, -1.0f, 1.0f));
			m_GameObjects.emplace(pointLight.GetID(), std::move(pointLight));
		}
	}
}
