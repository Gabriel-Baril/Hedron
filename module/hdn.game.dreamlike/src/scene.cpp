#include "scene.h"

#include "hobj/scene/transform_component.h"
#include "ecs/components/color_component.h"
#include "ecs/components/point_light_component.h"
#include "ecs/components/physics_component.h"
#include "ecs/components/model_component.h"

#include "application.h"

#include "core/utils.h"

namespace hdn
{
	RuntimeScene::RuntimeScene()
	{
		m_EcsWorld.set_threads(std::thread::hardware_concurrency());
	}

	void RuntimeScene::init()
	{
		m_UpdateTransformSystem = Application::get_system<UpdateTransformSystem>(NAME_UPDATE_TRANSFORM_SYSTEM);
		m_UpdateScriptSystem = Application::get_system<UpdateScriptSystem>(NAME_UPDATE_SCRIPT_SYSTEM);
		m_SimpleRenderSystem = Application::get_system<SimpleRenderSystem>(NAME_SIMPLE_RENDER_SYSTEM);
		m_PointLightSystem = Application::get_system<PointLightSystem>(NAME_POINT_LIGHT_SYSTEM);
		m_PhysicsGameObjectSystem = Application::get_system<PhysicsGameObjectSystem>(NAME_PHYSICS_GAMEOBJECT_SYSTEM);
		m_PhysicsWorldSystem = Application::get_system<PhysicsWorldSystem>(NAME_PHYSICS_WORLD_SYSTEM);
	}

	bool RuntimeScene::on_window_resized(WindowResizedEvent& event)
	{
		return false;
	}

	void RuntimeScene::load_example_scene()
	{
		VulkanDevice& device = Application::get().get_device();

		Ref<VulkanModel> hdnModel = VulkanModel::create_model_from_obj_file(&device, get_data_path("models/flat_vase.obj"));
		
		auto flatVaseGroup = create_entity("Flat Vase Group");
		TransformComponent transformC;
		flatVaseGroup.set(transformC);

		for (int i = 0; i < 100; i++)
		{
			std::string eName = fmt::format("Vase {}", i);

			auto flatVase = create_entity(eName.c_str());

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
			physicsC.physicsActor = m_PhysicsWorldSystem->create_dynamic_actor(position, dimension);
			flatVase.set(physicsC);

			flatVase.get_entity().child_of(flatVaseGroup.get_entity());
		}

		{
			hdnModel = VulkanModel::create_model_from_obj_file(&device, get_data_path("models/quad.obj"));
			auto floor = create_entity("floor");

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
			physicsC.physicsActor = m_PhysicsWorldSystem->create_static_actor(position, dimension);
			floor.set(physicsC);

			// floor.add_native_script<SimpleLogScript>();
			// floor.AddNativeScript<RotateZScript>();
		}

		{
			hdnModel = VulkanModel::create_model_from_fbx_file(&device, get_data_path("models/cube.fbx")); // models/cube.fbx

			auto pot = create_entity("pot");

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

		for (int i = 0; i < lightColors.size(); i++)
		{
			auto pointLight = Entity::make_point_light(m_EcsWorld, 0.2f);

			ColorComponent* colorC = pointLight.get_mut<ColorComponent>();
			colorC->color = lightColors[i];

			auto rotateLight = glm::rotate(mat4f32(1.0f), (i * glm::two_pi<f32>()) / lightColors.size(), { 0.0f, -1.0f, 0.0f });
			TransformComponent* transformC = pointLight.get_mut<TransformComponent>();
			transformC->position = vec3f32(rotateLight * vec4f32(-1.0f, -1.0f, -1.0f, 1.0f));
		}
	}

	void RuntimeScene::update(FrameInfo& frameInfo)
	{
		m_UpdateTransformSystem->update(m_EcsWorld);
		m_UpdateScriptSystem->update(frameInfo, m_EcsWorld);
		m_PointLightSystem->update(frameInfo, m_EcsWorld);
		m_PhysicsWorldSystem->update(frameInfo.frameTime);
		m_PhysicsGameObjectSystem->update(frameInfo, m_EcsWorld);
	}

	void RuntimeScene::render(FrameInfo& frameInfo)
	{
		m_SimpleRenderSystem->render(frameInfo, m_EcsWorld);
		m_PointLightSystem->render(frameInfo, m_EcsWorld);
	}

	Entity RuntimeScene::create_entity(const char* name)
	{
		flecs::entity e = m_EcsWorld.entity(name);
		return Entity{ e };
	}
}
