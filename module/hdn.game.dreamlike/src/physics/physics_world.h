#pragma once

#include "core/core.h"
#include <PxPhysicsAPI.h>

namespace hdn
{
	class PhysicsWorld
	{
	public:
		void Init();
		physx::PxRigidDynamic* CreateDynamicActor(const physx::PxVec3& position, const physx::PxVec3& dimension);
		physx::PxRigidStatic* CreateStaticActor(const physx::PxVec3& position, const physx::PxVec3& dimension);
		void Update(f32 deltaTime);
		void Shutdown();
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