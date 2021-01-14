#include "hdrpch.h"
#include "Hedron/Scene/Scene.h"

#include "Hedron/Renderer/Renderer2D.h"
#include "Hedron/Scene/Components.h"

#include <glm/glm.hpp>

namespace Hedron
{

	static void on_transform_construct()
	{
	}

	Scene::Scene()
	{
#if ENTT_EXAMPLE
		struct MeshComponent
		{
			float value;

			MeshComponent() = default;
			MeshComponent(const MeshComponent&) = default;
		};

		struct TransformComponent
		{
			glm::mat4 transform;

			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const glm::mat4& transform)
				: transform{ transform } {}

			operator glm::mat4& () { return transform; }
			operator const glm::mat4& () const { return transform; }
		};

		entt::entity entity = m_registry.create(); // entt::entity == uint32_t
		auto& transform = m_registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
		transform = glm::mat4(2.0f);

		m_registry.on_construct<TransformComponent>().connect<&on_transform_construct>();

		if (m_registry.has<TransformComponent>(entity)) // Check if entity has a TransformComponent
			TransformComponent& transform = m_registry.get<TransformComponent>(entity); // Retrieve this TransformComponent

		auto view = m_registry.view<TransformComponent>(); // Get all the entities with a TransformComponent
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity); // Retrieve this TransformComponent
		}

		auto group = m_registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto& entity : group)
		{
			auto&[transform, mesh] = group.get<TransformComponent, MeshComponent>(entity); // Retrieve this TransformComponent
		}
#endif
	}

	Scene::~Scene()
	{

	}

	void Scene::on_update(Timestep ts)
	{
		auto group = m_registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto& entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity); // Retrieve this TransformComponent
			Renderer2D::fill_rect(transform, sprite.color);
		}
	}

	entt::entity Scene::create_entity()
	{
		return m_registry.create();
	}
}