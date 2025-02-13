#pragma once

#include "core/core.h"
#include "core/stl/unordered_map.h"

#include "hdn_model.h"

#include <PxPhysicsAPI.h>

#include "flecs/flecs.h"

namespace hdn
{
	struct TransformComponent
	{
		vec3f32 translation{};
		vec3f32 scale{ 1.0f, 1.0f, 1.0f };
		vec3f32 rotation{}; // In radians (Euler YXZ)

		mat4f32 Mat4();
		mat3f32 NormalMatrix();
	};

	struct ColorComponent
	{
		vec3f32 color{};
	};

	struct PointLightComponent
	{
		float lightIntensity = 1.0f;
	};

	struct ModelComponent
	{
		Ref<HDNModel> model{};
	};

	struct PhysicsComponent
	{
		physx::PxRigidActor* physicsActor;
	};

	// TODO: We should use entt instead
	class HDNGameObject
	{
	public:
		using id_t = flecs::entity_t;

		HDNGameObject(const HDNGameObject&) = delete;
		HDNGameObject& operator=(const HDNGameObject&) = delete;
		HDNGameObject(HDNGameObject&&) = default;
		HDNGameObject& operator=(HDNGameObject&&) = default;

		static HDNGameObject CreateGameObject(flecs::world& ecs, const char* name = nullptr)
		{
			flecs::entity e = ecs.entity(name);
			return HDNGameObject{ e };
		}

		static HDNGameObject MakePointLight(flecs::world& ecs, f32 intensity = 10.0f, f32 radius = 0.1f, vec3f32 color = vec3f32{1.0f});
		
		template<typename T>
		const T* Get()
		{
			return m_Entity.get<T>();
		}

		template<typename T>
		T* GetMut()
		{
			return m_Entity.get_mut<T>();
		}

		template<typename T>
		void Set(const T& component)
		{
			m_Entity.set(component);
		}

		flecs::entity GetEntity()
		{
			return m_Entity;
		}

		id_t GetID()
		{
			return m_Entity.id();
		}

		const char* Name()
		{
			return m_Entity.name();
		}
	private:
		HDNGameObject(flecs::entity e) : m_Entity{ e } {}
		flecs::entity m_Entity;
	};
}