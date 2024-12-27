#include "physics_world.h"

using namespace physx;

namespace hdn
{
	void PhysicsWorld::Init()
	{
		m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_AllocatorCallback, m_ErrorCallback);
		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale{}, true);
		m_Dispatcher = PxDefaultCpuDispatcherCreate(2);
		PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		sceneDesc.cpuDispatcher = m_Dispatcher;
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;

		m_Scene = m_Physics->createScene(sceneDesc);
		m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.5f);
	}

	PxRigidDynamic* PhysicsWorld::CreateDynamicActor(const PxVec3& position, const PxVec3& dimension)
	{
		PxTransform transform(position);
		PxShape* shape = m_Physics->createShape(PxBoxGeometry(dimension), *m_Material);
		PxRigidDynamic* dynamicActor = m_Physics->createRigidDynamic(transform);
		dynamicActor->attachShape(*shape);
		shape->release();
		PxRigidBodyExt::updateMassAndInertia(*dynamicActor, 10.0f);
		m_Scene->addActor(*dynamicActor);
		return dynamicActor;
	}

	void PhysicsWorld::Update(f32 deltaTime)
	{
		m_Scene->simulate(deltaTime);
		m_Scene->fetchResults(true);
	}

	void PhysicsWorld::Shutdown()
	{
		m_Material->release();
		m_Scene->release();
		m_Dispatcher->release();
		m_Physics->release();
		m_Foundation->release();
	}
}
