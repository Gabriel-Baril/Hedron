#pragma once

#include "Core/Core.h"

#include "HDNModel.h"

namespace hdn
{
	struct Transform2DComponent
	{
		vec2f32 translation{};
		vec2f32 scale{1.0f, 1.0f};
		float32 rotation;

		mat2f32 mat2()
		{
			const float32 s = glm::sin(rotation);
			const float32 c = glm::cos(rotation);
			
			mat2f32 rotationMat{ {c, s}, {-s, c} };

			mat2f32 scaleMat{ {scale.x, 0.0f}, {0.0f, scale.y} }; // Column initializer
			return rotationMat * scaleMat;
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
		Transform2DComponent transform2d;

	private:
		HDNGameObject(id_t objId) : m_ID{ objId } {}
		id_t m_ID;
	};
}