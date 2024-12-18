#pragma once

#include "Core/Core.h"

#include "HDNModel.h"

#include <unordered_map>

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

		mat4f32 mat4();
		mat3f32 NormalMatrix();
	};

	// TODO: We should use entt instead
	class HDNGameObject
	{
	public:
		using id_t = uint32;
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

		id_t GetID() const { return m_ID; }

		Ref<HDNModel> model{};
		vec3f32 color{};
		TransformComponent transform{};

	private:
		HDNGameObject(id_t objId) : m_ID{ objId } {}
		id_t m_ID;
	};
}