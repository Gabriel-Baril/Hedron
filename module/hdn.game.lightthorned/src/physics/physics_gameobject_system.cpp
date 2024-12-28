#include "physics_gameobject_system.h"

#include "PxPhysicsAPI.h"

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
		for (auto& [key, obj] : *frameInfo.gameObjects)
		{
			if (obj.physicsComponent == nullptr || obj.physicsComponent->physicsActor == nullptr) continue;
			physx::PxTransform pxTransform = obj.physicsComponent->physicsActor->getGlobalPose();
			obj.transform.translation = glm::vec3(pxTransform.p.x, pxTransform.p.y, -pxTransform.p.z);
			const glm::quat rotQuat = glm::quat(pxTransform.q.w, pxTransform.q.x, pxTransform.q.y, pxTransform.q.z);
			obj.transform.rotation = QuaternionToEulerAngles(rotQuat);
		}
	}
}
