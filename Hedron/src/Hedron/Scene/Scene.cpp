#include "hdrpch.h"
#include "Hedron/Scene/Scene.h"

#include <glm/glm.hpp>

#include "Hedron/Renderer/Renderer2D.h"
#include "Hedron/Scene/Components.h"

#include "Hedron/Scene/Entity.h"


namespace Hedron
{
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
		// update scripts
		m_registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nativeScriptComponent)
		{
			// TODO: Move to Scene::OnPlay
			if (!nativeScriptComponent.instance)
			{
				nativeScriptComponent.instance = nativeScriptComponent.instantiateScript();
				nativeScriptComponent.instance->m_entity = Entity{ entity, this };
				nativeScriptComponent.instance->on_create();
			}
			nativeScriptComponent.instance->on_update(ts);
		});

		// Render sprites
		Camera* mainCamera = nullptr;
		glm::mat4 camTransform;
		{
			auto view = m_registry.view<TransformComponent, CameraComponent>();
			for (auto& entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.primary)
				{
					mainCamera = &camera.camera;
					camTransform = transform.get_transform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::begin_scene(mainCamera->get_projection(), camTransform);

			auto group = m_registry.group<TransformComponent, SpriteRendererComponent>();
			for (auto& entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity); // Retrieve this TransformComponent
				Renderer2D::fill_rect(transform.get_transform(), sprite.color);
			}

			Renderer2D::end_scene();
		}
	}

	Entity Scene::create_entity(const std::string& name)
	{
		entt::entity entityId = m_registry.create();
		Entity entity = { entityId, this };
		entity.add_component<TransformComponent>();
		auto& tag = entity.add_component<TagComponent>();
		if (name.empty())
			tag.tag = "Entity" + std::to_string((uint32_t)entityId);
		else
			tag.tag = name;
		return entity;
	}

	void Scene::destroy_entity(Entity entity)
	{
		m_registry.destroy(entity);
	}

	void Scene::on_viewport_resize(uint32_t width, uint32_t height)
	{
		m_viewportWidth = width;
		m_viewportHeight = height;

		// Resize our non-fixed aspect ratio camera
		auto view = m_registry.view<CameraComponent>();
		for (auto& entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.fixedAspectRatio)
				cameraComponent.camera.set_viewport_size(width, height);
		}
	}

	template<typename Component>
	void Scene::on_component_added(Entity& entity, Component& addComponent)
	{
		static_assert(false);
	}

	template<>
	void Scene::on_component_added<TransformComponent>(Entity& entity, TransformComponent& transformComponent)
	{
	}

	template<>
	void Scene::on_component_added<CameraComponent>(Entity& entity, CameraComponent& cameraComponent)
	{
		cameraComponent.camera.set_viewport_size(m_viewportWidth, m_viewportHeight);
	}

	template<>
	void Scene::on_component_added<TagComponent>(Entity& entity, TagComponent& tagComponent)
	{
	}

	template<>
	void Scene::on_component_added<SpriteRendererComponent>(Entity& entity, SpriteRendererComponent& spriteRendererComponent)
	{
	}

	template<>
	void Scene::on_component_added<NativeScriptComponent>(Entity& entity, NativeScriptComponent& nativeScriptComponent)
	{
	}
}