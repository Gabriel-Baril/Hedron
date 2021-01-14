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
	private:
		entt::registry m_registry; // Contains all of our entities

		friend class Entity;
	};
}