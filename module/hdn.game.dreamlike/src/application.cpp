#include "application.h"

#include "keyboard_movement_controller.h"
#include "camera.h"

#include "systems/system_simple_render.h"
#include "systems/system_point_light.h"
#include "systems/system_physics_gameobject.h"
#include "systems/system_update_transform.h"
#include "systems/system_update_script.h"
#include "systems/system_physics_world.h"
#include "systems/system_imgui.h"

#include "ecs/scripts/script_simple_log.h"
#include "ecs/scripts/script_rotate_z.h"

#include "gfx/r_vk_buffer.h"

#include "core/core.h"
#include <glm/gtc/constants.hpp>

#include "hobj/hobj_registry.h"
#include "hobj/scene/scene_hobj.h"
#include "systems/system_camera.h"

namespace hdn
{
	static constexpr f32 MAX_FRAME_TIME = 0.5f;

	Application::Application()
	{
		HINFO("IN");
		m_Scene = make_ref<RuntimeScene>();
		m_GlobalPool = VulkanDescriptorPool::Builder(m_Device)
			.set_max_sets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT) // The maximum amount of sets in the pools
			.add_pool_size(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkanSwapChain::MAX_FRAMES_IN_FLIGHT) // The number of uniform descriptor in the descriptor pool
			.build();

		Ref<PhysicsWorldSystem> physicsWorld = m_SystemRegistry.register_system<PhysicsWorldSystem>(NAME_PHYSICS_WORLD_SYSTEM);
		physicsWorld->init();
	}

	Application::~Application()
	{
		m_SystemRegistry.unregister_system(NAME_CAMERA_SYSTEM);
		m_SystemRegistry.unregister_system(NAME_SIMPLE_RENDER_SYSTEM);
		m_SystemRegistry.unregister_system(NAME_POINT_LIGHT_SYSTEM);
		m_SystemRegistry.unregister_system(NAME_PHYSICS_GAMEOBJECT_SYSTEM);
		m_SystemRegistry.unregister_system(NAME_UPDATE_TRANSFORM_SYSTEM);
		m_SystemRegistry.unregister_system(NAME_UPDATE_SCRIPT_SYSTEM);
		m_SystemRegistry.unregister_system(NAME_PHYSICS_WORLD_SYSTEM);
	}

	Application& Application::get()
	{
		static Application s_Instance{};
		return s_Instance;
	}

	void Application::on_event(Event& event)
	{
		IApplication::on_event(event);

		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowResizedEvent>(HDN_BIND_EVENT_FN(Application::on_window_resized));
		dispatcher.dispatch<KeyPressedEvent>(HDN_BIND_EVENT_FN(Application::on_key_pressed));
		dispatcher.dispatch<MouseButtonPressedEvent>(HDN_BIND_EVENT_FN(Application::on_mouse_button_pressed));
	}

	bool Application::on_window_resized(WindowResizedEvent& event)
	{
		if (m_Scene)
		{
			m_Scene->on_window_resized(event);
		}
		return false;
	}

	bool Application::on_key_pressed(KeyPressedEvent& event)
	{
		return false;
	}

	bool Application::on_mouse_button_pressed(MouseButtonPressedEvent& event)
	{
		return false;
	}

	void Application::run()
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

		auto currentTime = std::chrono::high_resolution_clock::now();

#if USING(HDN_DEBUG)
		ImguiSystem imguiSystem;
		
		m_ImguiDescriptorPool = VulkanDescriptorPool::Builder(m_Device)
			.set_pool_flags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
			.set_max_sets(1)
			.add_pool_size(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)
			.build();
		
		m_QueueFamilyIndices = m_Device.find_physical_queue_families();
		imguiSystem.init(
			m_Window.get_glfw_window(),
			m_Device.get_surface(),
			m_Device.get_instance(),
			m_Device.get_physical_device(),
			m_Device.get_device(),
			m_QueueFamilyIndices.graphicsFamily,
			m_Device.get_graphics_queue(),
			m_ImguiDescriptorPool->get_descriptor()
		);
#endif

		m_SystemRegistry.register_system<SimpleRenderSystem>(NAME_SIMPLE_RENDER_SYSTEM, &m_Device, m_Renderer.get_swap_chain_render_pass(), m_GlobalSetLayout->get_descriptor_set_layout());
		m_SystemRegistry.register_system<PointLightSystem>(NAME_POINT_LIGHT_SYSTEM, &m_Device, m_Renderer.get_swap_chain_render_pass(), m_GlobalSetLayout->get_descriptor_set_layout());
		m_SystemRegistry.register_system<PhysicsGameObjectSystem>(NAME_PHYSICS_GAMEOBJECT_SYSTEM);
		m_SystemRegistry.register_system<UpdateTransformSystem>(NAME_UPDATE_TRANSFORM_SYSTEM);
		m_SystemRegistry.register_system<UpdateScriptSystem>(NAME_UPDATE_SCRIPT_SYSTEM);

		m_Scene->init();
		m_Scene->load_example_scene();

		Ref<CameraSystem> cameraSystem = m_SystemRegistry.register_system<CameraSystem>(NAME_CAMERA_SYSTEM);
		cameraSystem->init();

		while (!m_Window.should_close())
		{
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			f32 frameTime = std::chrono::duration<f32, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			frameTime = glm::min(frameTime, MAX_FRAME_TIME);

			cameraSystem->update(frameTime);

			if (auto commandBuffer = m_Renderer.begin_frame())
			{
				GlobalUbo ubo{};
				ubo.projection = cameraSystem->get_camera().get_projection();
				ubo.view = cameraSystem->get_camera().get_view();
				ubo.inverseView = cameraSystem->get_camera().get_inverse_view();

				int frameIndex = m_Renderer.get_frame_index();
				FrameInfo frameInfo{};
				frameInfo.frameIndex = frameIndex;
				frameInfo.frameTime = frameTime;
				frameInfo.commandBuffer = commandBuffer;
				frameInfo.globalDescriptorSet = m_GlobalDescriptorSets[frameIndex];
				frameInfo.ubo = &ubo;

				// update

				m_Scene->update(frameInfo);

				m_UboBuffers[frameIndex]->write_to_buffer((void*)&ubo);
				m_UboBuffers[frameIndex]->flush();

				// render
				m_Renderer.begin_swap_chain_render_pass(commandBuffer);

				// Order Here Matters
				m_Scene->render(frameInfo);

#if USING(HDN_DEBUG)
				imguiSystem.begin_frame();
				ImGui::Begin("Hello, world!");
				ImGui::Text("This is some useful text.");
				ImGui::Text("dt: %.4f", frameTime * 1000);
				ImGui::End();
				imguiSystem.end_frame(ImVec4(0.45f, 0.55f, 0.60f, 1.00f), commandBuffer);
#endif
				m_Renderer.end_swap_chain_render_pass(commandBuffer);

				m_Renderer.end_frame();
			}
		}

		vkDeviceWaitIdle(m_Device.get_device());
	}
}
