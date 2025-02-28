#pragma once

#include "core/core.h"

#include "flecs/flecs.h"

#include "editor_panel.h"

namespace hdn
{
	class OutlinerPanel : public IEditorPanel
	{
	public:
		OutlinerPanel(flecs::world* ecs)
			: m_Ecs{ ecs }, IEditorPanel{ "outliner" }
		{
		}

		void OnUpdate(f32 dt);
	private:
		void DrawEntityTree(flecs::entity entity, flecs::world& ecs);
	private:
		flecs::world* m_Ecs = nullptr;
	};
}