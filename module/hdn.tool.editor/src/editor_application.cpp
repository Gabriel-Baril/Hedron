#include "editor_application.h"

#include "ecs/components/transform_component.h"
#include "ecs/components/color_component.h"
#include "ecs/components/point_light_component.h"
#include "ecs/components/physics_component.h"
#include "ecs/components/model_component.h"

#include "ecs/scripts/simple_log_script.h"
#include "ecs/scripts/rotate_z_script.h"

#include "panel/editor_panel_outliner.h"
#include "panel/editor_panel_module_manager.h"
#include "panel/editor_panel_ideation_manager.h"
#include "panel/editor_panel_performance.h"
#include "panel/editor_panel_inspector.h"

#include "r_vk_buffer.h"

#include "core/core.h"
#include <glm/gtc/constants.hpp>

namespace hdn
{
	static constexpr f32 MAX_FRAME_TIME = 0.5f;

	EditorApplication::EditorApplication()
	{
		m_GlobalPool = VulkanDescriptorPool::Builder(m_Device)
			.SetMaxSets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT) // The maximum amount of sets in the pools
			.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkanSwapChain::MAX_FRAMES_IN_FLIGHT) // The number of uniform descriptor in the descriptor pool
			.Build();

		m_PhysicsWorld.Init();
		LoadGameObjects();

		m_EcsWorld.set_threads(std::thread::hardware_concurrency());

		Register<OutlinerPanel>(&m_EcsWorld);
		Register<InspectorPanel>(&m_EcsWorld);
		Register<ModuleManagerPanel>();
		Register<IdeationManagerPanel>();
		Register<PerformancePanel>();
	}

	EditorApplication::~EditorApplication()
	{
		m_PhysicsWorld.Shutdown();
	}

	void EditorApplication::Run()
	{
		for (int i = 0;i < m_UboBuffers.size(); i++)
		{
			m_UboBuffers[i] = CreateScope<VulkanBuffer>(
				&m_Device,
				sizeof(GlobalUbo),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);
			m_UboBuffers[i]->Map();
		}

		m_GlobalSetLayout = VulkanDescriptorSetLayout::Builder(m_Device)
			.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
			.Build();

		for (int i = 0;i < m_GlobalDescriptorSets.size(); i++)
		{
			auto bufferInfo = m_UboBuffers[i]->DescriptorInfo();
			VulkanDescriptorWriter(*m_GlobalSetLayout, *m_GlobalPool)
				.WriteBuffer(0, &bufferInfo)
				.Build(m_GlobalDescriptorSets[i]);
		}

		m_SimpleRenderSystem.Init(&m_Device, m_Renderer.GetSwapChainRenderPass(), m_GlobalSetLayout->GetDescriptorSetLayout());
		m_PointLightSystem.Init(&m_Device, m_Renderer.GetSwapChainRenderPass(), m_GlobalSetLayout->GetDescriptorSetLayout());

		m_ViewerObject = HDNGameObject::CreateGameObject(m_EcsWorld);
		TransformComponent transformC;
		transformC.translation.z = -2.5f;
		m_ViewerObject.GetEntity().set(transformC);

		auto currentTime = std::chrono::high_resolution_clock::now();

		m_ImguiDescriptorPool = VulkanDescriptorPool::Builder(m_Device)
			.SetPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
			.SetMaxSets(1)
			.AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)
			.Build();
		
		m_QueueFamilyIndices = m_Device.FindPhysicalQueueFamilies();
		m_ImguiSystem.Init(
			m_Window.GetGLFWWindow(),
			m_Device.GetSurface(),
			m_Device.GetInstance(),
			m_Device.GetPhysicalDevice(),
			m_Device.GetDevice(),
			m_QueueFamilyIndices.graphicsFamily,
			m_Device.GetGraphicsQueue(),
			m_ImguiDescriptorPool->GetDescriptor()
		);

		while (!m_Window.ShouldClose())
		{
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<f32, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			frameTime = glm::min(frameTime, MAX_FRAME_TIME);

			m_CameraController.MoveInPlaneXZ(m_Window.GetGLFWWindow(), frameTime, m_ViewerObject);

			TransformComponent* viewerObjectTransformC = m_ViewerObject.GetMut<TransformComponent>();
			m_Camera.SetViewYXZ(viewerObjectTransformC->translation, viewerObjectTransformC->rotation);

			f32 aspect = m_Renderer.GetAspectRatio();
			m_Camera.SetPerspectiveProjection(glm::radians(50.0f), aspect, 0.01f, 1.0f);

			if (auto commandBuffer = m_Renderer.BeginFrame())
			{
				int frameIndex = m_Renderer.GetFrameIndex();
				FrameInfo frameInfo{};
				frameInfo.frameIndex = frameIndex;
				frameInfo.frameTime = frameTime;
				frameInfo.commandBuffer = commandBuffer;
				frameInfo.camera = &m_Camera;
				frameInfo.globalDescriptorSet = m_GlobalDescriptorSets[frameIndex];
				frameInfo.ecsWorld = &m_EcsWorld;

				// update
				GlobalUbo ubo{};
				ubo.projection = m_Camera.GetProjection();
				ubo.view = m_Camera.GetView();
				ubo.inverseView = m_Camera.GetInverseView();

				m_UpdateTransformSystem.Update(frameInfo);
				m_UpdateScriptSystem.Update(frameInfo);
				m_PointLightSystem.Update(frameInfo, ubo);
				m_PhysicsWorld.Update(frameTime);
				m_PhysicsGameObjectSystem.Update(frameInfo);

				m_UboBuffers[frameIndex]->WriteToBuffer((void*)&ubo);
				m_UboBuffers[frameIndex]->Flush();

				// render
				m_Renderer.BeginSwapChainRenderPass(commandBuffer);

				// Order Here Matters
				m_SimpleRenderSystem.RenderGameObjects(frameInfo);
				m_PointLightSystem.Render(frameInfo);

				m_ImguiSystem.BeginFrame();

				// ImGui::ShowDemoWindow();

				for (const auto& panel : m_Panels)
				{
					if (!panel->Enabled())
					{
						continue;
					}
					panel->Begin();
					panel->OnUpdate(frameInfo.frameTime);
					panel->End();
				}

				m_ImguiSystem.EndFrame(ImVec4(0.45f, 0.55f, 0.60f, 1.00f), commandBuffer);

				m_Renderer.EndSwapChainRenderPass(commandBuffer);

				m_Renderer.EndFrame();
			}
		}

		vkDeviceWaitIdle(m_Device.GetDevice());
	}

	void EditorApplication::LoadGameObjects()
	{
		Ref<VulkanModel> hdnModel = VulkanModel::CreateModelFromObjFile(&m_Device, "models/flat_vase.obj");

		{
			auto flatVaseGroup = HDNGameObject::CreateGameObject(m_EcsWorld, "Flat Vase Group");
			TransformComponent transformC;
			flatVaseGroup.Set(transformC);

			for (int i = 0; i < 0; i++)
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
			auto cube = HDNGameObject::CreateGameObject(m_EcsWorld, "cube");

			TransformComponent transformC;
			transformC.translation = { 0.0f, 0.0f, 0.0f };
			transformC.scale = vec3f32{ 1.0f, 1.0f, 1.0f };
			cube.Set(transformC);

			ModelComponent modelC;
			modelC.model = hdnModel;
			cube.Set(modelC);
		}

		{
			vector<vec3f32> lightColors{
				{1.f, .1f, .1f},
				{.1f, .1f, 1.f},
				{.1f, 1.f, .1f},
				{1.f, 1.f, .1f},
				{.1f, 1.f, 1.f},
				{1.f, 1.f, 1.f}
			};

			auto pointLightGroup = HDNGameObject::CreateGameObject(m_EcsWorld, "pointLightGroup");
			TransformComponent transformC;
			pointLightGroup.Set(transformC);

			for (int i = 0; i < lightColors.size(); i++)
			{
				auto pointLight = HDNGameObject::MakePointLight(m_EcsWorld, 0.2f);

				ColorComponent* colorC = pointLight.GetMut<ColorComponent>();
				colorC->color = lightColors[i];

				auto rotateLight = glm::rotate(mat4f32(1.0f), (i * glm::two_pi<f32>()) / lightColors.size(), { 0.0f, -1.0f, 0.0f });
				TransformComponent* transformC = pointLight.GetMut<TransformComponent>();
				transformC->translation = vec3f32(rotateLight * vec4f32(-1.0f, -1.0f, -1.0f, 1.0f));

				pointLight.GetEntity().child_of(pointLightGroup.GetEntity());
			}
		}
	}
}
