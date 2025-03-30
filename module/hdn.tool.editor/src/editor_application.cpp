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

#include "input/input.h"
#include "editor.h"

namespace hdn
{
	static constexpr f32 MAX_FRAME_TIME = 0.5f;

	EditorApplication& EditorApplication::get()
	{
		static EditorApplication s_Instance{};
		return s_Instance;
	}

	void EditorApplication::on_event(Event& event)
	{
		IApplication::on_event(event);
		m_EditorCamera.OnEvent(event);
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowResizedEvent>(HDN_BIND_EVENT_FN(EditorApplication::on_window_resized));
		dispatcher.dispatch<KeyPressedEvent>(HDN_BIND_EVENT_FN(EditorApplication::on_key_pressed));
		dispatcher.dispatch<MouseButtonPressedEvent>(HDN_BIND_EVENT_FN(EditorApplication::on_mouse_button_pressed));
	}

	bool EditorApplication::on_window_resized(WindowResizedEvent& event)
	{
		m_EditorCamera.SetViewportSize(event.get_width(), event.get_height());
		return false;
	}

	bool EditorApplication::on_key_pressed(KeyPressedEvent& event)
	{
		// HINFO("EditorApplication::OnKeyPressed - Pressed!");
		return false;
	}

	bool EditorApplication::on_mouse_button_pressed(MouseButtonPressedEvent& event)
	{
		return false;
	}

	EditorApplication::EditorApplication()
	{
		m_ActiveScene = make_scope<Scene>();
		Editor::Get().SetActiveScene(m_ActiveScene);

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
		m_EditorCamera.SetViewportSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

		m_Window.SetEventCallback(HDN_BIND_EVENT_FN(EditorApplication::on_event));

		m_GlobalPool = VulkanDescriptorPool::Builder(m_Device)
			.SetMaxSets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT) // The maximum amount of sets in the pools
			.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkanSwapChain::MAX_FRAMES_IN_FLIGHT) // The number of uniform descriptor in the descriptor pool
			.Build();

		m_PhysicsWorld.Init();

		load_game_objects();

		register_panel<OutlinerPanel>();
		register_panel<InspectorPanel>();
		register_panel<ModuleManagerPanel>();
		register_panel<IdeationManagerPanel>();
		register_panel<PerformancePanel>();
		m_ViewportPanel = register_panel<ViewportPanel>();
		m_ViewportPanel->CreateOffscreenRenderTarget(m_Device.GetDevice(), m_Device.GetPhysicalDevice(), m_Window.GetExtent(), m_Renderer.GetSwapChainRenderPass());
		m_ViewportPanel->CreateDescriptorSet(m_Device.GetDevice());
	}

	EditorApplication::~EditorApplication()
	{
		m_PhysicsWorld.Shutdown();
	}

	void EditorApplication::run()
	{
		for (int i = 0;i < m_UboBuffers.size(); i++)
		{
			m_UboBuffers[i] = make_scope<VulkanBuffer>(
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
			Timestep frameTime = std::chrono::duration<f32, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			frameTime = glm::min(frameTime.seconds(), MAX_FRAME_TIME);

			EditorCamera::UpdateState state;
			state.LeftAltPressed = Input::GetKey(KeyCode::LeftAlt);
			state.MouseButtonLeftPressed = Input::GetMouseButton(MouseButton::MouseButtonLeft);
			state.MouseButtonMiddlePressed = Input::GetMouseButton(MouseButton::MouseButtonMiddle);
			state.MouseButtonRightPressed = Input::GetMouseButton(MouseButton::MouseButtonRight);
			state.MousePosition = Input::GetMousePosition();
			m_EditorCamera.OnUpdate(frameTime, state);

			if (auto commandBuffer = m_Renderer.BeginFrame())
			{
				int frameIndex = m_Renderer.GetFrameIndex();
				FrameInfo frameInfo{};
				frameInfo.frameIndex = frameIndex;
				frameInfo.frameTime = frameTime;
				frameInfo.commandBuffer = commandBuffer;
				frameInfo.camera = &m_EditorCamera;
				frameInfo.globalDescriptorSet = m_GlobalDescriptorSets[frameIndex];
				frameInfo.scene = m_ActiveScene.get();

				// update
				GlobalUbo ubo{};
				ubo.projection = m_EditorCamera.GetProjectionMatrix();
				ubo.view = m_EditorCamera.GetViewMatrix();
				ubo.inverseView = m_EditorCamera.GetInverseViewMatrix();

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
				m_ViewportPanel->TransitionImage(commandBuffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
				
				m_SimpleRenderSystem.RenderGameObjects(frameInfo);
				m_PointLightSystem.Render(frameInfo);

				m_ViewportPanel->TransitionImage(commandBuffer, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

				// Render Scene to texture
				m_ViewportPanel->UpdateDescriptorSet(m_Device.GetDevice(), frameIndex);

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

	void EditorApplication::load_game_objects()
	{
		Ref<VulkanModel> hdnModel = VulkanModel::CreateModelFromObjFile(&m_Device, "models/flat_vase.obj");

		{
			auto flatVaseGroup = m_ActiveScene->Create("Flat Vase Group");
			TransformComponent transformC;
			flatVaseGroup.Set(transformC);

			for (int i = 0; i < 0; i++)
			{
				std::string eName = fmt::format("Vase {}", i);

				auto flatVase = m_ActiveScene->Create(eName.c_str()); // TODO: Fix game object lifetime

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
			auto floor = m_ActiveScene->Create("floor");

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
			auto cube = m_ActiveScene->Create("cube");

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

			auto pointLightGroup = m_ActiveScene->Create("pointLightGroup");
			TransformComponent transformC;
			pointLightGroup.Set(transformC);

			for (int i = 0; i < lightColors.size(); i++)
			{
				auto pointLight = m_ActiveScene->MakePointLight(0.2f);

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
