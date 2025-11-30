#pragma once
#include "core/core.h"
#include "core/hash.h"

#include "model_db.h"
#include "physics_db.h"

namespace hdn
{
	struct TransformComponent
	{
		// TODO: Change to mat4 only
		vec3f32 position{};
		vec3f32 scale{ 1.0f, 1.0f, 1.0f };
		vec3f32 rotation{}; // In radians (Euler YXZ)

		mat4f32 worldMatrix; // Populated at runtime

		mat4f32 to_mat();
		mat3f32 normal_matrix();
	};

	struct ModelComponent
	{
		ModelAsset* model;
	};

	struct ColorComponent
	{
		vec3f32 color{};
	};

	struct PointLightComponent
	{
		float lightIntensity = 1.0f;
	};

	struct NativeScriptComponent
	{
		u64 scripTypeHash;
		// TODO: We could later add script parameters here (for example a kvp store)
	};

	struct PhysicsComponent
	{
		PhysicsActorAsset* physicsActor;
	};

	enum SerializedComponentType
	{
		TRANSFORM,
		MODEL,
		COLOR,
		POINT_LIGHT,
		NATIVE_SCRIPT,
		PHYSICS,
		SIZE
	};

	struct ComponentDef
	{
		SerializedComponentType type;
		union
		{
			TransformComponent* transformC;
			ModelComponent* modelC;
			ColorComponent* colorC;
			PointLightComponent* pointLightC;
			NativeScriptComponent* nativeScriptC;
			PhysicsComponent* physicsC;
			void* data;
		};
	};

	struct PrefabDef
	{
		u64 id;
		int componentCount;
		int prefabCount;
		ComponentDef* components;
		PrefabDef* parentPrefab = nullptr;
		PrefabDef* childPrefabs = nullptr;
	};

	// This asset is used to instantiate a entity
	struct EntityDef
	{
		u64 id;
		PrefabDef* prefab = nullptr;
	};

	// This asset is used to instantiate a scene (flecs::world)
	struct SceneDef
	{
		h64 name;
		int entityCount;
		EntityDef* entities; // Only top level entities
	};
}