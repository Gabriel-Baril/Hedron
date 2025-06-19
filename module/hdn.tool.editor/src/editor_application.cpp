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
#include "core/utils.h"

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
		m_EditorCamera.on_event(event);
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowResizedEvent>(HDN_BIND_EVENT_FN(EditorApplication::on_window_resized));
		dispatcher.dispatch<KeyPressedEvent>(HDN_BIND_EVENT_FN(EditorApplication::on_key_pressed));
		dispatcher.dispatch<MouseButtonPressedEvent>(HDN_BIND_EVENT_FN(EditorApplication::on_mouse_button_pressed));
	}

	bool EditorApplication::on_window_resized(WindowResizedEvent& event)
	{
		m_EditorCamera.set_viewport_size(event.get_width(), event.get_height());
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
		Editor::get().set_active_scene(m_ActiveScene);

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
		m_EditorCamera.set_viewport_size(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

		m_Window.SetEventCallback(HDN_BIND_EVENT_FN(EditorApplication::on_event));

		m_GlobalPool = VulkanDescriptorPool::Builder(m_Device)
			.set_max_sets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT) // The maximum amount of sets in the pools
			.add_pool_size(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkanSwapChain::MAX_FRAMES_IN_FLIGHT) // The number of uniform descriptor in the descriptor pool
			.build();

		m_PhysicsWorld.init();

		load_game_objects();

		register_panel<OutlinerPanel>();
		register_panel<InspectorPanel>();
		register_panel<ModuleManagerPanel>();
		register_panel<IdeationManagerPanel>();
		register_panel<PerformancePanel>();
		m_ViewportPanel = register_panel<ViewportPanel>();
		m_ViewportPanel->create_offscreen_rendertarget(m_Device.get_device(), m_Device.get_physical_device(), m_Window.get_extent(), m_Renderer.get_swap_chain_render_pass());
		m_ViewportPanel->create_descriptor_set(m_Device.get_device());
	}

	EditorApplication::~EditorApplication()
	{
		m_PhysicsWorld.shutdown();
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
			m_UboBuffers[i]->map();
		}

		m_GlobalSetLayout = VulkanDescriptorSetLayout::Builder(m_Device)
			.add_binding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
			.build();

		for (int i = 0;i < m_GlobalDescriptorSets.size(); i++)
		{
			auto bufferInfo = m_UboBuffers[i]->build_descriptor_info();
			VulkanDescriptorWriter(*m_GlobalSetLayout, *m_GlobalPool)
				.write_buffer(0, &bufferInfo)
				.build(m_GlobalDescriptorSets[i]);
		}

		m_SimpleRenderSystem.Init(&m_Device, m_Renderer.get_swap_chain_render_pass(), m_GlobalSetLayout->get_descriptor_set_layout());
		m_PointLightSystem.Init(&m_Device, m_Renderer.get_swap_chain_render_pass(), m_GlobalSetLayout->get_descriptor_set_layout());

		auto currentTime = std::chrono::high_resolution_clock::now();

		m_ImguiDescriptorPool = VulkanDescriptorPool::Builder(m_Device)
			.set_pool_flags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
			.set_max_sets(1)
			.add_pool_size(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)
			.build();
		
		m_QueueFamilyIndices = m_Device.find_physical_queue_families();
		m_ImguiSystem.init(
			m_Window.get_glfw_window(),
			m_Device.get_surface(),
			m_Device.get_instance(),
			m_Device.get_physical_device(),
			m_Device.get_device(),
			m_QueueFamilyIndices.graphicsFamily,
			m_Device.get_graphics_queue(),
			m_ImguiDescriptorPool->get_descriptor()
		);

		while (!m_Window.should_close())
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
			m_EditorCamera.on_update(frameTime, state);

			if (auto commandBuffer = m_Renderer.begin_frame())
			{
				int frameIndex = m_Renderer.get_frame_index();
				FrameInfo frameInfo{};
				frameInfo.frameIndex = frameIndex;
				frameInfo.frameTime = frameTime;
				frameInfo.commandBuffer = commandBuffer;
				frameInfo.camera = &m_EditorCamera;
				frameInfo.globalDescriptorSet = m_GlobalDescriptorSets[frameIndex];
				frameInfo.scene = m_ActiveScene.get();

				// update
				GlobalUbo ubo{};
				ubo.projection = m_EditorCamera.get_projection_matrix();
				ubo.view = m_EditorCamera.get_view_matrix();
				ubo.inverseView = m_EditorCamera.get_inverse_view_matrix();

				m_UpdateTransformSystem.update(frameInfo);
				m_UpdateScriptSystem.update(frameInfo);
				m_PointLightSystem.update(frameInfo, ubo);
				m_PhysicsWorld.update(frameTime);
				m_PhysicsGameObjectSystem.update(frameInfo);

				m_UboBuffers[frameIndex]->write_to_buffer((void*)&ubo);
				m_UboBuffers[frameIndex]->flush();

				// render
				m_Renderer.begin_swap_chain_render_pass(commandBuffer);

				// Order Here Matters
				m_ViewportPanel->transition_image(commandBuffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
				
				m_SimpleRenderSystem.render(frameInfo);
				m_PointLightSystem.render(frameInfo);

				m_ViewportPanel->transition_image(commandBuffer, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

				// Render Scene to texture
				m_ViewportPanel->update_descriptor_set(m_Device.get_device(), frameIndex);

				m_ImguiSystem.begin_frame();

				// ImGui::ShowDemoWindow();

				for (const auto& panel : m_Panels)
				{
					if (!panel->enabled())
					{
						continue;
					}
					panel->begin();
					panel->on_update(frameInfo.frameTime);
					panel->end();
				}

				m_ImguiSystem.end_frame(ImVec4(0.45f, 0.55f, 0.60f, 1.00f), commandBuffer);

				m_Renderer.end_swap_chain_render_pass(commandBuffer);

				m_Renderer.end_frame();
			}
		}

		vkDeviceWaitIdle(m_Device.get_device());
	}

	void EditorApplication::load_game_objects()
	{
		Ref<VulkanModel> hdnModel = VulkanModel::create_model_from_obj_file(&m_Device, get_data_path("models/flat_vase.obj"));

		{
			auto flatVaseGroup = m_ActiveScene->create("Flat Vase Group");
			TransformComponent transformC;
			flatVaseGroup.Set(transformC);

			for (int i = 0; i < 0; i++)
			{
				std::string eName = fmt::format("Vase {}", i);

				auto flatVase = m_ActiveScene->create(eName.c_str()); // TODO: Fix game object lifetime

				TransformComponent transformC;
				transformC.position = { cos(i), -1 - (float)sin(i), sin(i) };
				transformC.scale = vec3f32{ 1.0f, 1.0f, 1.0f };
				flatVase.Set(transformC);

				ModelComponent modelC;
				modelC.model = hdnModel;
				flatVase.Set(modelC);

				PhysicsComponent physicsC;
				physx::PxVec3 position = physx::PxVec3(transformC.position.x, transformC.position.y, -transformC.position.z);
				physx::PxVec3 dimension = physx::PxVec3(0.1f, 0.2f, 0.1f);
				physicsC.physicsActor = m_PhysicsWorld.create_dynamic_actor(position, dimension);
				flatVase.Set(physicsC);

				flatVase.GetEntity().child_of(flatVaseGroup.GetEntity());
			}
		}

		{
			hdnModel = VulkanModel::create_model_from_obj_file(&m_Device, get_data_path("models/quad.obj"));
			auto floor = m_ActiveScene->create("floor");

			TransformComponent transformC;
			transformC.position = { 0.0f, 2.0f, 0.0f };
			transformC.scale = vec3f32{ 3.0f, 1.0f, 3.0f };
			floor.Set(transformC);

			ModelComponent modelC;
			modelC.model = hdnModel;
			floor.Set(modelC);

			PhysicsComponent physicsC;
			physx::PxVec3 position = physx::PxVec3(transformC.position.x, transformC.position.y, -transformC.position.z);
			physx::PxVec3 dimension = physx::PxVec3(3.0f, 0.001f, 3.0f);
			physicsC.physicsActor = m_PhysicsWorld.create_static_actor(position, dimension);
			floor.Set(physicsC);

			floor.AddNativeScript<SimpleLogScript>();
			// floor.AddNativeScript<RotateZScript>();
		}

		{
			hdnModel = VulkanModel::create_model_from_fbx_file(&m_Device, get_data_path("models/cube.fbx")); // models/cube.fbx
			auto cube = m_ActiveScene->create("cube");

			TransformComponent transformC;
			transformC.position = { 0.0f, 0.0f, 0.0f };
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

			auto pointLightGroup = m_ActiveScene->create("pointLightGroup");
			TransformComponent transformC;
			pointLightGroup.Set(transformC);

			for (int i = 0; i < lightColors.size(); i++)
			{
				auto pointLight = m_ActiveScene->make_point_light(0.2f);

				ColorComponent* colorC = pointLight.GetMut<ColorComponent>();
				colorC->color = lightColors[i];

				auto rotateLight = glm::rotate(mat4f32(1.0f), (i * glm::two_pi<f32>()) / lightColors.size(), { 0.0f, -1.0f, 0.0f });
				TransformComponent* transformC = pointLight.GetMut<TransformComponent>();
				transformC->position = vec3f32(rotateLight * vec4f32(-1.0f, -1.0f, -1.0f, 1.0f));

				pointLight.GetEntity().child_of(pointLightGroup.GetEntity());
			}
		}
	}
}
