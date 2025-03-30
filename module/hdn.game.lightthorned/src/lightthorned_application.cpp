#include "lightthorned_application.h"

#include "keyboard_movement_controller.h"
#include "camera.h"

#include "ecs/components/transform_component.h"
#include "ecs/components/color_component.h"
#include "ecs/components/point_light_component.h"
#include "ecs/components/physics_component.h"
#include "ecs/components/model_component.h"

#include "ecs/systems/simple_render_system.h"
#include "ecs/systems/point_light_system.h"
#include "ecs/systems/physics_gameobject_system.h"
#include "ecs/systems/update_transform_system.h"
#include "ecs/systems/update_script_system.h"

#include "ecs/scripts/simple_log_script.h"
#include "ecs/scripts/rotate_z_script.h"

#include "r_vk_imgui.h"
#include "r_vk_buffer.h"

#include "core/core.h"
#include <glm/gtc/constants.hpp>

namespace hdn
{
	static constexpr f32 MAX_FRAME_TIME = 0.5f;

	LightthornedApplication::LightthornedApplication()
	{
		m_GlobalPool = VulkanDescriptorPool::Builder(m_Device)
			.SetMaxSets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT) // The maximum amount of sets in the pools
			.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkanSwapChain::MAX_FRAMES_IN_FLIGHT) // The number of uniform descriptor in the descriptor pool
			.Build();

		m_PhysicsWorld.Init();
		LoadGameObjects();

		m_EcsWorld.set_threads(std::thread::hardware_concurrency());
	}

	LightthornedApplication::~LightthornedApplication()
	{
		m_PhysicsWorld.Shutdown();
	}

	void LightthornedApplication::Run()
	{
		vector<Scope<VulkanBuffer>> uboBuffers(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0;i < uboBuffers.size(); i++)
		{
			uboBuffers[i] = make_scope<VulkanBuffer>(
				&m_Device,
				sizeof(GlobalUbo),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);
			uboBuffers[i]->Map();
		}

		auto globalSetLayout = VulkanDescriptorSetLayout::Builder(m_Device)
			.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
			.Build();

		vector<VkDescriptorSet> globalDescriptorSets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT); // One descriptor set per frame
		for (int i = 0;i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->DescriptorInfo();
			VulkanDescriptorWriter(*globalSetLayout, *m_GlobalPool)
				.WriteBuffer(0, &bufferInfo)
				.Build(globalDescriptorSets[i]);
		}

		UpdateTransformSystem updateTransformSystem;
		UpdateScriptSystem updateScriptSystem;
		SimpleRenderSystem simpleRenderSystem{ &m_Device, m_Renderer.GetSwapChainRenderPass(), globalSetLayout->GetDescriptorSetLayout() };
		PointLightSystem pointLightSystem{ &m_Device, m_Renderer.GetSwapChainRenderPass(), globalSetLayout->GetDescriptorSetLayout() };
		PhysicsGameObjectSystem physicsGameObjectSystem;
		HDNCamera camera{};

		auto viewerObject = HDNGameObject::CreateGameObject(m_EcsWorld);
		TransformComponent transformC;
		transformC.translation.z = -2.5f;
		viewerObject.GetEntity().set(transformC);

		KeyboardMovementController cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

#if USING(HDN_DEBUG)
		ImguiSystem imguiSystem;
		
		Scope<VulkanDescriptorPool> imguiDescriptorPool = VulkanDescriptorPool::Builder(m_Device)
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
#endif

		while (!m_Window.ShouldClose())
		{
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<f32, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			frameTime = glm::min(frameTime, MAX_FRAME_TIME);

			cameraController.MoveInPlaneXZ(m_Window.GetGLFWWindow(), frameTime, viewerObject);

			TransformComponent* viewerObjectTransformC = viewerObject.GetMut<TransformComponent>();
			camera.SetViewYXZ(viewerObjectTransformC->translation, viewerObjectTransformC->rotation);

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
				frameInfo.ecsWorld = &m_EcsWorld;

				// update
				GlobalUbo ubo{};
				ubo.projection = camera.GetProjection();
				ubo.view = camera.GetView();
				ubo.inverseView = camera.GetInverseView();

				updateTransformSystem.Update(frameInfo);
				updateScriptSystem.Update(frameInfo);
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

				imguiSystem.EndFrame(ImVec4(0.45f, 0.55f, 0.60f, 1.00f), commandBuffer);
#endif
				m_Renderer.EndSwapChainRenderPass(commandBuffer);

				m_Renderer.EndFrame();
			}
		}

		vkDeviceWaitIdle(m_Device.GetDevice());
	}

	void LightthornedApplication::LoadGameObjects()
	{
		Ref<VulkanModel> hdnModel = VulkanModel::CreateModelFromObjFile(&m_Device, "models/flat_vase.obj");

		auto flatVaseGroup = HDNGameObject::CreateGameObject(m_EcsWorld, "Flat Vase Group");
		TransformComponent transformC;
		flatVaseGroup.Set(transformC);

		for(int i = 0;i < 100; i++)
		{
			std::string eName = fmt::format("Vase {}", i);

			auto flatVase = HDNGameObject::CreateGameObject(m_EcsWorld, eName.c_str()); // TODO: Fix game object lifetime

			TransformComponent transformC;
			transformC.translation = { cos(i), -1 - (float)sin(i), sin(i) };
			transformC.scale = vec3f32{ 1.0f, 1.0f, 1.0f };
			flatVase.Set(transformC);

			ModelComponent modelC;
			modelC.model = hdnModel;
			flatVase.Set(modelC);

			PhysicsComponent physicsC;
			physx::PxVec3 position = physx::PxVec3(transformC.translation.x, transformC.translation.y, -transformC.translation.z);
			physx::PxVec3 dimension = physx::PxVec3(0.1f, 0.2f, 0.1f);
			physicsC.physicsActor = m_PhysicsWorld.CreateDynamicActor(position, dimension);
			flatVase.Set(physicsC);

			flatVase.GetEntity().child_of(flatVaseGroup.GetEntity());
		}

		{
			hdnModel = VulkanModel::CreateModelFromObjFile(&m_Device, "models/quad.obj");
			auto floor = HDNGameObject::CreateGameObject(m_EcsWorld, "floor");

			TransformComponent transformC;
			transformC.translation = { 0.0f, 2.0f, 0.0f };
			transformC.scale = vec3f32{ 3.0f, 1.0f, 3.0f };
			floor.Set(transformC);

			ModelComponent modelC;
			modelC.model = hdnModel;
			floor.Set(modelC);

			PhysicsComponent physicsC;
			physx::PxVec3 position = physx::PxVec3(transformC.translation.x, transformC.translation.y, -transformC.translation.z);
			physx::PxVec3 dimension = physx::PxVec3(3.0f, 0.001f, 3.0f);
			physicsC.physicsActor = m_PhysicsWorld.CreateStaticActor(position, dimension);
			floor.Set(physicsC);

			floor.AddNativeScript<SimpleLogScript>();
			// floor.AddNativeScript<RotateZScript>();
		}

		{
			hdnModel = VulkanModel::CreateModelFromFbxFile(&m_Device, "models/cube.fbx"); // models/cube.fbx
			auto pot = HDNGameObject::CreateGameObject(m_EcsWorld, "pot");

			TransformComponent transformC;
			transformC.translation = { 0.0f, 0.0f, 0.0f };
			transformC.scale = vec3f32{ 1.0f, 1.0f, 1.0f };
			pot.Set(transformC);

			ModelComponent modelC;
			modelC.model = hdnModel;
			pot.Set(modelC);
		}

		vector<vec3f32> lightColors{
			{1.f, .1f, .1f},
			{.1f, .1f, 1.f},
			{.1f, 1.f, .1f},
			{1.f, 1.f, .1f},
			{.1f, 1.f, 1.f},
			{1.f, 1.f, 1.f} 
		};

		for (int i = 0;i < lightColors.size(); i++)
		{
			auto pointLight = HDNGameObject::MakePointLight(m_EcsWorld, 0.2f);

			ColorComponent* colorC = pointLight.GetMut<ColorComponent>();
			colorC->color = lightColors[i];

			auto rotateLight = glm::rotate(mat4f32(1.0f), (i * glm::two_pi<f32>()) / lightColors.size(), {0.0f, -1.0f, 0.0f});
			TransformComponent* transformC = pointLight.GetMut<TransformComponent>();
			transformC->translation = vec3f32(rotateLight * vec4f32(-1.0f, -1.0f, -1.0f, 1.0f));
		}
	}
}
