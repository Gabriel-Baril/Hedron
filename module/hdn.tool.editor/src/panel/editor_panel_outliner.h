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

		void OnUpdate(f32 dt);
	private:
		void DrawEntityTree(flecs::entity entity, flecs::world& ecs);
	};
}