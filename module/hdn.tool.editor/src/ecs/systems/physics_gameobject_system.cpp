#include "physics_gameobject_system.h"

#include "PxPhysicsAPI.h"

#include "ecs/components/transform_component.h"
#include "ecs/components/physics_component.h"

namespace hdn
{
	glm::vec3 QuaternionToEulerAngles(const glm::quat& q) {
		glm::vec3 euler;

		// Yaw (rotation around Y-axis)
		float siny_cosp = 2.0f * (q.w * q.y + q.z * q.x);
		float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.x * q.x);
		euler.y = std::atan2(siny_cosp, cosy_cosp);

		// Pitch (rotation around X-axis)
		float sinp = 2.0f * (q.w * q.x - q.y * q.z);
		if (std::abs(sinp) >= 1.0f)
			euler.x = std::copysign(glm::half_pi<float>(), sinp); // Clamp to 90 degrees if out of bounds
		else
			euler.x = std::asin(sinp);

		// Roll (rotation around Z-axis)
		float sinr_cosp = 2.0f * (q.w * q.z + q.x * q.y);
		float cosr_cosp = 1.0f - 2.0f * (q.z * q.z + q.x * q.x);
		euler.z = std::atan2(sinr_cosp, cosr_cosp);

		return euler;
	}

	void PhysicsGameObjectSystem::Update(FrameInfo& frameInfo)
	{

		auto query = frameInfo.scene->World()->query<TransformComponent, PhysicsComponent>();
		query.each([&](flecs::entity e, TransformComponent& transformC, PhysicsComponent& physicsC) {
			if (physicsC.physicsActor == nullptr)
			{
				return;
			}

			physx::PxTransform pxTransform = physicsC.physicsActor->getGlobalPose();
			transformC.position = glm::vec3(pxTransform.p.x, pxTransform.p.y, -pxTransform.p.z);
			const glm::quat rotQuat = glm::quat(pxTransform.q.w, pxTransform.q.x, pxTransform.q.y, pxTransform.q.z);
			transformC.rotation = QuaternionToEulerAngles(rotQuat);
		});
	}
}
