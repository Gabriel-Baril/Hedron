#pragma once

#include "core/core.h"
#include "flecs/flecs.h"
#include "entity.h"

namespace hdn
{
	class Scene
	{
	public:
		Scene();

		Entity Create(const char* name = nullptr)
		{
			flecs::entity e = m_Registry.entity(name);
			return Entity{ e };
		}

		Entity MakePointLight(f32 intensity = 10.0f, f32 radius = 0.1f, vec3f32 color = vec3f32{ 1.0f });

		flecs::world* World() { return &m_Registry; }
	private:
		flecs::world m_Registry;
	};
}