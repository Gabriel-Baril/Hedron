#include "VulkanPlayground/FirstApp.h"

#include "VulkanPlayground/KeyboardMovementController.h"
#include "VulkanPlayground/HDNBuffer.h"
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

	// Global Uniform Buffer Object
	struct GlobalUbo
	{
		alignas(16) mat4f32 projectionView{ 1.0f };
		alignas(16) vec3f32 lightDirection = glm::normalize(vec3f32{ 1.0f, -3.0f, -1.0f });
	};

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
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(HDNSwapChain::MAX_FRAMES_IN_FLIGHT); // One descriptor set per frame
		for (int i = 0;i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			HDNDescriptorWriter(*globalSetLayout, *globalPool)
				.writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}

		SimpleRenderSystem simpleRenderSystem{ &m_Device, m_Renderer.GetSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
		HDNCamera camera{};

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
			camera.SetPerspectiveProjection(glm::radians(50.0f), aspect, 0.01f, 1.0f);

			if (auto commandBuffer = m_Renderer.BeginFrame())
			{
				int frameIndex = m_Renderer.GetFrameIndex();
				FrameInfo frameInfo{};
				frameInfo.frameIndex = frameIndex;
				frameInfo.frameTime = frameTime;
				frameInfo.commandBuffer = commandBuffer;
				frameInfo.camera = &camera;
				frameInfo.globalDescriptorSet = globalDescriptorSets[frameIndex];

				// update
				GlobalUbo ubo{};
				ubo.projectionView = camera.GetProjection() * camera.GetView();
				uboBuffers[frameIndex]->writeToBuffer((void*)&ubo);
				uboBuffers[frameIndex]->flush();

				// render
				m_Renderer.BeginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.RenderGameObjects(frameInfo, m_GameObjects);
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
		flatVase.transform.translation = { 0.0f, 0.0f, 2.5f };
		flatVase.transform.scale = vec3f32{3.0f, 1.0f, 2.0f};
		m_GameObjects.push_back(std::move(flatVase));

		hdnModel = HDNModel::CreateModelFromFile(&m_Device, "Models/smooth_vase.obj");
		auto smoothVase = HDNGameObject::CreateGameObject();
		smoothVase.model = hdnModel;
		smoothVase.transform.translation = { -1.0f, 0.0f, 2.5f };
		smoothVase.transform.scale = vec3f32{ 1.0f, 1.0f, 1.0f };
		m_GameObjects.push_back(std::move(smoothVase));
	}
}
