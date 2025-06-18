#pragma once

#include "core/core.h"
#include <PxPhysicsAPI.h>

namespace hdn
{
	class PhysicsWorld
	{
	public:
		void init();
		physx::PxRigidDynamic* create_dynamic_actor(const physx::PxVec3& position, const physx::PxVec3& dimension);
		physx::PxRigidStatic* create_static_actor(const physx::PxVec3& position, const physx::PxVec3& dimension);
		void update(f32 deltaTime);
		void shutdown();
	private:
		physx::PxFoundation* m_Foundation = nullptr;
		physx::PxPhysics* m_Physics = nullptr;
		physx::PxDefaultCpuDispatcher* m_Dispatcher = nullptr;
		physx::PxScene* m_Scene = nullptr;
		physx::PxMaterial* m_Material = nullptr;
		physx::PxDefaultAllocator m_AllocatorCallback;
		physx::PxDefaultErrorCallback m_ErrorCallback;
	};
}