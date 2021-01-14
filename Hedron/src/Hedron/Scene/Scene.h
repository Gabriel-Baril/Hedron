#pragma once
#include <entt.hpp>

#include "Hedron/Core/Timestep.h"

namespace Hedron
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity create_entity(); // TODO: temp

		// TEMP
		entt::registry& reg() { return m_registry; }

		void on_update(Timestep ts);
	private:
		entt::registry m_registry; // Contains all of our entities
	};
}