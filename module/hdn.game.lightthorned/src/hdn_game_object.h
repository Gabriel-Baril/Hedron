#pragma once

#include "core/core.h"

#include "hdn_model.h"

#include <PxPhysicsAPI.h>
#include <unordered_map>

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

	struct PointLightComponent
	{
		float lightIntensity = 1.0f;
	};

	struct PhysicsComponent
	{
		physx::PxRigidActor* physicsActor;
	};

	// TODO: We should use entt instead
	class HDNGameObject
	{
	public:
		using id_t = u32;
		using Map = std::unordered_map<id_t, HDNGameObject>;

		HDNGameObject(const HDNGameObject&) = delete;
		HDNGameObject& operator=(const HDNGameObject&) = delete;
		HDNGameObject(HDNGameObject&&) = default;
		HDNGameObject& operator=(HDNGameObject&&) = default;

		static HDNGameObject CreateGameObject()
		{
			static id_t currentId = 0;
			return HDNGameObject{ currentId++ };
		}

		static HDNGameObject MakePointLight(f32 intensity = 10.0f, f32 radius = 0.1f, vec3f32 color = vec3f32{1.0f});

		id_t GetID() const { return m_ID; }

		std::string name;
		vec3f32 color{};
		TransformComponent transform{};
		
		// Optional Pointer Components
		Ref<HDNModel> model{};
		Scope<PointLightComponent> pointLight = nullptr;
		Scope<PhysicsComponent> physicsComponent = nullptr;

	private:
		HDNGameObject(id_t objId) : m_ID{ objId } {}
		id_t m_ID;
	};
}