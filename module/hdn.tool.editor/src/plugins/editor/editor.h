#pragma once

#include "core/core.h"

#include "flecs/flecs.h"

namespace hdn
{
	class Editor
	{
	public:
		void Init();

		void DrawEntityTree(flecs::entity entity, flecs::world& ecs);
		void RenderEntityTable(flecs::world& ecs);
	};
}