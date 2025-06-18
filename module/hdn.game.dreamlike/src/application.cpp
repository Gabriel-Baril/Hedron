#include "application.h"

#include "keyboard_movement_controller.h"
#include "camera.h"

#include "hobj/scene/transform_component.h"
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

#include "hobj/hobj_registry.h"
#include "hobj/scene/scene_hobj.h"
#include "core/utils.h"

namespace hdn
{
	static constexpr f32 MAX_FRAME_TIME = 0.5f;

	Application::Application()
	{
		m_GlobalPool = VulkanDescriptorPool::Builder(m_Device)
			.set_max_sets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT) // The maximum amount of sets in the pools
			.add_pool_size(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkanSwapChain::MAX_FRAMES_IN_FLIGHT) // The number of uniform descriptor in the descriptor pool
			.build();

		m_PhysicsWorld.init();
		load_game_objects();

		m_EcsWorld.set_threads(std::thread::hardware_concurrency());
	}

	Application::~Application()
	{
		m_PhysicsWorld.shutdown();
	}

	void Application::run()
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
			uboBuffers[i]->map();
		}

		auto globalSetLayout = VulkanDescriptorSetLayout::Builder(m_Device)
			.add_binding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
			.build();

		vector<VkDescriptorSet> globalDescriptorSets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT); // One descriptor set per frame
		for (int i = 0;i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->build_descriptor_info();
			VulkanDescriptorWriter(*globalSetLayout, *m_GlobalPool)
				.write_buffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}

		UpdateTransformSystem updateTransformSystem;
		UpdateScriptSystem updateScriptSystem;
		SimpleRenderSystem simpleRenderSystem{ &m_Device, m_Renderer.get_swap_chain_render_pass(), globalSetLayout->get_descriptor_set_layout() };
		PointLightSystem pointLightSystem{ &m_Device, m_Renderer.get_swap_chain_render_pass(), globalSetLayout->get_descriptor_set_layout() };
		PhysicsGameObjectSystem physicsGameObjectSystem;
		HDNCamera camera{};

		auto viewerObject = HDNGameObject::create_game_object(m_EcsWorld);
		TransformComponent transformC;
		transformC.position.z = -2.5f;
		viewerObject.get_entity().set(transformC);

		KeyboardMovementController cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

#if USING(HDN_DEBUG)
		ImguiSystem imguiSystem;
		
		Scope<VulkanDescriptorPool> imguiDescriptorPool = VulkanDescriptorPool::Builder(m_Device)
			.set_pool_flags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
			.set_max_sets(1)
			.add_pool_size(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)
			.build();
		
		QueueFamilyIndices queueFamilyIndices = m_Device.find_physical_queue_families();
		imguiSystem.init(
			m_Window.get_glfw_window(),
			m_Device.get_surface(),
			m_Device.get_instance(),
			m_Device.get_physical_device(),
			m_Device.get_device(),
			queueFamilyIndices.graphicsFamily,
			m_Device.get_graphics_queue(),
			imguiDescriptorPool->get_descriptor()
		);
#endif

		while (!m_Window.should_close())
		{
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<f32, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			frameTime = glm::min(frameTime, MAX_FRAME_TIME);

			cameraController.move_in_plane_xyz(m_Window.get_glfw_window(), frameTime, viewerObject);

			TransformComponent* viewerObjectTransformC = viewerObject.get_mut<TransformComponent>();
			camera.set_view_yxz(viewerObjectTransformC->position, viewerObjectTransformC->rotation);

			f32 aspect = m_Renderer.get_aspect_ratio();
			camera.set_perspective_projection(glm::radians(50.0f), aspect, 0.01f, 1.0f);

			if (auto commandBuffer = m_Renderer.begin_frame())
			{
				int frameIndex = m_Renderer.get_frame_index();
				FrameInfo frameInfo{};
				frameInfo.frameIndex = frameIndex;
				frameInfo.frameTime = frameTime;
				frameInfo.commandBuffer = commandBuffer;
				frameInfo.camera = &camera;
				frameInfo.globalDescriptorSet = globalDescriptorSets[frameIndex];
				frameInfo.ecsWorld = &m_EcsWorld;

				// update
				GlobalUbo ubo{};
				ubo.projection = camera.get_projection();
				ubo.view = camera.get_view();
				ubo.inverseView = camera.get_inverse_view();

				updateTransformSystem.update(frameInfo);
				updateScriptSystem.update(frameInfo);
				pointLightSystem.update(frameInfo, ubo);
				m_PhysicsWorld.update(frameTime);
				physicsGameObjectSystem.update(frameInfo);

				uboBuffers[frameIndex]->write_to_buffer((void*)&ubo);
				uboBuffers[frameIndex]->flush();

				// render
				m_Renderer.begin_swap_chain_render_pass(commandBuffer);

				// Order Here Matters
				simpleRenderSystem.render_game_objects(frameInfo);
				pointLightSystem.render(frameInfo);

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

	void Application::load_game_objects()
	{
		Ref<VulkanModel> hdnModel = VulkanModel::create_model_from_obj_file(&m_Device, get_data_path("models/flat_vase.obj"));

		auto flatVaseGroup = HDNGameObject::create_game_object(m_EcsWorld, "Flat Vase Group");
		TransformComponent transformC;
		flatVaseGroup.set(transformC);

		for(int i = 0;i < 100; i++)
		{
			std::string eName = fmt::format("Vase {}", i);

			auto flatVase = HDNGameObject::create_game_object(m_EcsWorld, eName.c_str()); // TODO: Fix game object lifetime

			TransformComponent transformC;
			transformC.position = { cos(i), -1 - (float)sin(i), sin(i) };
			transformC.scale = vec3f32{ 1.0f, 1.0f, 1.0f };
			flatVase.set(transformC);

			ModelComponent modelC;
			modelC.model = hdnModel;
			flatVase.set(modelC);

			PhysicsComponent physicsC;
			physx::PxVec3 position = physx::PxVec3(transformC.position.x, transformC.position.y, -transformC.position.z);
			physx::PxVec3 dimension = physx::PxVec3(0.1f, 0.2f, 0.1f);
			physicsC.physicsActor = m_PhysicsWorld.create_dynamic_actor(position, dimension);
			flatVase.set(physicsC);

			flatVase.get_entity().child_of(flatVaseGroup.get_entity());
		}

		{
			hdnModel = VulkanModel::create_model_from_obj_file(&m_Device, get_data_path("models/quad.obj"));
			auto floor = HDNGameObject::create_game_object(m_EcsWorld, "floor");

			TransformComponent transformC;
			transformC.position = { 0.0f, 2.0f, 0.0f };
			transformC.scale = vec3f32{ 3.0f, 1.0f, 3.0f };
			floor.set(transformC);

			ModelComponent modelC;
			modelC.model = hdnModel;
			floor.set(modelC);

			PhysicsComponent physicsC;
			physx::PxVec3 position = physx::PxVec3(transformC.position.x, transformC.position.y, -transformC.position.z);
			physx::PxVec3 dimension = physx::PxVec3(3.0f, 0.001f, 3.0f);
			physicsC.physicsActor = m_PhysicsWorld.create_static_actor(position, dimension);
			floor.set(physicsC);

			floor.add_native_script<SimpleLogScript>();
			// floor.AddNativeScript<RotateZScript>();
		}

		{
			hdnModel = VulkanModel::create_model_from_fbx_file(&m_Device, get_data_path("models/cube.fbx")); // models/cube.fbx
			auto pot = HDNGameObject::create_game_object(m_EcsWorld, "pot");

			TransformComponent transformC;
			transformC.position = { 0.0f, 0.0f, 0.0f };
			transformC.scale = vec3f32{ 1.0f, 1.0f, 1.0f };
			pot.set(transformC);

			ModelComponent modelC;
			modelC.model = hdnModel;
			pot.set(modelC);
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
			auto pointLight = HDNGameObject::make_point_light(m_EcsWorld, 0.2f);

			ColorComponent* colorC = pointLight.get_mut<ColorComponent>();
			colorC->color = lightColors[i];

			auto rotateLight = glm::rotate(mat4f32(1.0f), (i * glm::two_pi<f32>()) / lightColors.size(), {0.0f, -1.0f, 0.0f});
			TransformComponent* transformC = pointLight.get_mut<TransformComponent>();
			transformC->position = vec3f32(rotateLight * vec4f32(-1.0f, -1.0f, -1.0f, 1.0f));
		}
	}
}
