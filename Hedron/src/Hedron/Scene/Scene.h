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
		void on_update(Timestep ts);

		void on_viewport_resize(uint32_t width, uint32_t height);
	private:
		entt::registry m_registry; // Contains all of our entities
		uint32_t m_viewportWidth = 0;
		uint32_t m_viewportHeight = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}