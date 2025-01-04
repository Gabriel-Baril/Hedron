#include "first_app.h"

#include "keyboard_movement_controller.h"
#include "hdn_buffer.h"
#include "hdn_camera.h"

#include "simple_render_system.h"
#include "point_light_system.h"
#include "physics/physics_gameobject_system.h"

#include "hdn_imgui.h"
#include "idaes/idaes_imgui.h"
#include "ideon/ideon_imgui.h"

#include "core/core.h"
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <chrono>

namespace hdn
{
	static constexpr f32 MAX_FRAME_TIME = 0.5f;

	FirstApp::FirstApp()
	{
		globalPool = HDNDescriptorPool::Builder(m_Device)
			.SetMaxSets(HDNSwapChain::MAX_FRAMES_IN_FLIGHT) // The maximum amount of sets in the pools
			.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, HDNSwapChain::MAX_FRAMES_IN_FLIGHT) // The number of uniform descriptor in the descriptor pool
			.Build();

		m_PhysicsWorld.Init();
		LoadGameObjects();
	}

	FirstApp::~FirstApp()
	{
		m_PhysicsWorld.Shutdown();
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
			uboBuffers[i]->Map();
		}

		auto globalSetLayout = HDNDescriptorSetLayout::Builder(m_Device)
			.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
			.Build();

		std::vector<VkDescriptorSet> globalDescriptorSets(HDNSwapChain::MAX_FRAMES_IN_FLIGHT); // One descriptor set per frame
		for (int i = 0;i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->DescriptorInfo();
			HDNDescriptorWriter(*globalSetLayout, *globalPool)
				.WriteBuffer(0, &bufferInfo)
				.Build(globalDescriptorSets[i]);
		}

		SimpleRenderSystem simpleRenderSystem{ &m_Device, m_Renderer.GetSwapChainRenderPass(), globalSetLayout->GetDescriptorSetLayout() };
		PointLightSystem pointLightSystem{ &m_Device, m_Renderer.GetSwapChainRenderPass(), globalSetLayout->GetDescriptorSetLayout() };
		PhysicsGameObjectSystem physicsGameObjectSystem;
		HDNCamera camera{};

		auto viewerObject = HDNGameObject::CreateGameObject();
		viewerObject.transform.translation.z = -2.5f;
		KeyboardMovementController cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

#if USING(HDN_DEBUG)
		ImguiSystem imguiSystem;
		
		Scope<HDNDescriptorPool> imguiDescriptorPool = HDNDescriptorPool::Builder(m_Device)
			.SetPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
			.SetMaxSets(1)
			.AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)
			.Build();
		
		QueueFamilyIndices queueFamilyIndices = m_Device.FindPhysicalQueueFamilies();
		imguiSystem.Init(
			m_Window.GetGLFWWindow(),
			m_Device.GetSurface(),
			m_Device.GetInstance(),
			m_Device.GetPhysicalDevice(),
			m_Device.GetDevice(),
			queueFamilyIndices.graphicsFamily,
			m_Device.GetGraphicsQueue(),
			imguiDescriptorPool->GetDescriptor()
		);

		IdaesImgui idaesUI;
		idaesUI.Init();

		IdeonImgui ideonUI;

		//ideonUI.LoadTestResultFile("test_results/test_result.xml");
#endif

		while (!m_Window.ShouldClose())
		{
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<f32, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			frameTime = glm::min(frameTime, MAX_FRAME_TIME);

			cameraController.MoveInPlaneXZ(m_Window.GetGLFWWindow(), frameTime, viewerObject);
			camera.SetViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			f32 aspect = m_Renderer.GetAspectRatio();
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
				frameInfo.gameObjects = &m_GameObjects;

				// update
				GlobalUbo ubo{};
				ubo.projection = camera.GetProjection();
				ubo.view = camera.GetView();
				ubo.inverseView = camera.GetInverseView();

				pointLightSystem.Update(frameInfo, ubo);
				m_PhysicsWorld.Update(frameTime);
				physicsGameObjectSystem.Update(frameInfo);

				uboBuffers[frameIndex]->WriteToBuffer((void*)&ubo);
				uboBuffers[frameIndex]->Flush();

				// render
				m_Renderer.BeginSwapChainRenderPass(commandBuffer);

				// Order Here Matters
				simpleRenderSystem.RenderGameObjects(frameInfo);
				pointLightSystem.Render(frameInfo);

#if USING(HDN_DEBUG)
				imguiSystem.BeginFrame();

				ImGui::Begin("Hello, world!");
				ImGui::Text("This is some useful text.");
				ImGui::Text("dt: %.4f", frameTime * 1000);
				ImGui::End();

				// ImGui::ShowDemoWindow();
				idaesUI.Draw();
				ideonUI.Draw();

				imguiSystem.EndFrame(ImVec4(0.45f, 0.55f, 0.60f, 1.00f), commandBuffer);
#endif
				m_Renderer.EndSwapChainRenderPass(commandBuffer);

				m_Renderer.EndFrame();
			}
		}

		vkDeviceWaitIdle(m_Device.GetDevice());
	}

	void FirstApp::LoadGameObjects()
	{
		Ref<HDNModel> hdnModel = HDNModel::CreateModelFromFile(&m_Device, "models/flat_vase.obj");

		for(int i = 0;i < 300; i++)
		{
			auto flatVase = HDNGameObject::CreateGameObject();
			flatVase.name = "vase";
			flatVase.model = hdnModel;
			flatVase.transform.translation = { cos(i), -1 - (float)sin(i), sin(i)};
			flatVase.transform.scale = vec3f32{ 1.0f, 1.0f, 1.0f };

			flatVase.physicsComponent = CreateScope<PhysicsComponent>();
			physx::PxVec3 position = physx::PxVec3(flatVase.transform.translation.x, flatVase.transform.translation.y, -flatVase.transform.translation.z);
			physx::PxVec3 dimension = physx::PxVec3(0.1f, 0.2f, 0.1f);
			flatVase.physicsComponent->physicsActor = m_PhysicsWorld.CreateDynamicActor(position, dimension);

			m_GameObjects.emplace(flatVase.GetID(), std::move(flatVase));
		}

		{
			hdnModel = HDNModel::CreateModelFromFile(&m_Device, "models/quad.obj");
			auto floor = HDNGameObject::CreateGameObject();
			floor.name = "floor";
			floor.model = hdnModel;
			floor.transform.translation = { 0.0f, 2.0f, 0.0f };
			floor.transform.scale = vec3f32{ 3.0f, 1.0f, 3.0f };

			floor.physicsComponent = CreateScope<PhysicsComponent>();
			physx::PxVec3 position = physx::PxVec3(floor.transform.translation.x, floor.transform.translation.y, -floor.transform.translation.z);
			physx::PxVec3 dimension = physx::PxVec3(3.0f, 0.001f, 3.0f);
			floor.physicsComponent->physicsActor = m_PhysicsWorld.CreateStaticActor(position, dimension);

			m_GameObjects.emplace(floor.GetID(), std::move(floor));
		}

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
			auto rotateLight = glm::rotate(mat4f32(1.0f), (i * glm::two_pi<f32>()) / lightColors.size(), {0.0f, -1.0f, 0.0f});
			pointLight.transform.translation = vec3f32(rotateLight * vec4f32(-1.0f, -1.0f, -1.0f, 1.0f));
			m_GameObjects.emplace(pointLight.GetID(), std::move(pointLight));
		}
	}
}
