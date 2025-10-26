#pragma once

#include "core/core.h"
#include "scene.h"
#include "editor_panel.h"

namespace hdn
{
	class OutlinerPanel : public IEditorPanel
	{
	public:
		OutlinerPanel()
			: IEditorPanel{ "outliner" }
		{
		}

		void on_update(f32 dt);
	private:
		void draw_entity_tree(flecs::entity entity, flecs::world& ecs);
	};
}