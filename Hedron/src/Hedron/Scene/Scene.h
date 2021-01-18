#pragma once
#include <entt.hpp>

#include "Hedron/Core/Timestep.h"

namespace Hedron
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity create_entity(const std::string& name = ""); // TODO: temp
		void destroy_entity(Entity entity);

		void on_update(Timestep ts);
		void on_viewport_resize(uint32_t width, uint32_t height);

		Entity get_primary_camera_entity();
	private:
		template<typename Component>
		void on_component_added(Entity& entity, Component& addComponent);
	private:
		entt::registry m_registry; // Contains all of our entities
		uint32_t m_viewportWidth = 0;
		uint32_t m_viewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}