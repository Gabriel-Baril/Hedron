#pragma once

#include "core/core.h"
#include "flecs/flecs.h"
#include "editor_panel.h"

namespace hdn
{
	class InspectorPanel : public IEditorPanel
	{
	public:
		InspectorPanel(flecs::world* ecs)
			: m_Ecs{ ecs }, IEditorPanel{ "inspector" }
		{
		}

		void OnUpdate(f32 dt);
	private:
		flecs::world* m_Ecs = nullptr;
	};
}