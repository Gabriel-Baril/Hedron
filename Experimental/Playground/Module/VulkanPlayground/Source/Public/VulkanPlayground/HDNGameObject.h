#pragma once

#include "Core/Core.h"

#include "HDNModel.h"

namespace hdn
{
	struct TransformComponent
	{
		vec3f32 translation{};
		vec3f32 scale{ 1.0f, 1.0f, 1.0f };
		vec3f32 rotation{}; // In radians

		// Matrix corresponds to translate * Ry * Rx * Rz * scale transformation
		// Rotation convention uses tait-bryan angles with axis order Y(1), X(2), Z(3)
		// mat4f32 mat4()
		// {
		// 	auto transform = glm::translate(mat4f32{ 1.0f }, translation);
		// 	transform = glm::rotate(transform, rotation.y, { 0.0f, 1.0f, 0.0f });
		// 	transform = glm::rotate(transform, rotation.x, { 1.0f, 0.0f, 0.0f });
		// 	transform = glm::rotate(transform, rotation.z, { 0.0f, 0.0f, 1.0f });
		// 	transform = glm::scale(transform, scale);
		// 	return transform;
		// }

		mat4f32 mat4() {
			const float32 c3 = glm::cos(rotation.z);
			const float32 s3 = glm::sin(rotation.z);
			const float32 c2 = glm::cos(rotation.x);
			const float32 s2 = glm::sin(rotation.x);
			const float32 c1 = glm::cos(rotation.y);
			const float32 s1 = glm::sin(rotation.y);
			return mat4f32{
				{
					scale.x * (c1 * c3 + s1 * s2 * s3),
					scale.x * (c2 * s3),
					scale.x * (c1 * s2 * s3 - c3 * s1),
					0.0f,
				},
				{
					scale.y * (c3 * s1 * s2 - c1 * s3),
					scale.y * (c2 * c3),
					scale.y * (c1 * c3 * s2 + s1 * s3),
					0.0f,
				},
				{
					scale.z * (c2 * s1),
					scale.z * (-s2),
					scale.z * (c1 * c2),
					0.0f,
				},
				{translation.x, translation.y, translation.z, 1.0f}
			};
		}
	};

	// TODO: We should use entt instead
	class HDNGameObject
	{
	public:
		using id_t = uint32;

		HDNGameObject(const HDNGameObject&) = delete;
		HDNGameObject& operator=(const HDNGameObject&) = delete;
		HDNGameObject(HDNGameObject&&) = default;
		HDNGameObject& operator=(HDNGameObject&&) = default;

		static HDNGameObject CreateGameObject()
		{
			static id_t currentId = 0;
			return HDNGameObject{ currentId++ };
		}

		id_t GetID() const { return m_ID; }

		Ref<HDNModel> model{};
		vec3f32 color{};
		TransformComponent transform{};

	private:
		HDNGameObject(id_t objId) : m_ID{ objId } {}
		id_t m_ID;
	};
}