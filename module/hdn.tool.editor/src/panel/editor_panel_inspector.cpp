#include "editor_panel_inspector.h"
#include "editor.h"
#include "ecs/components/transform_component.h"
#include "ecs/components/outline_renderer.h"

namespace hdn
{
	void InspectorPanel::OnUpdate(f32 dt)
	{
		if (!Editor::Get().HasEntitySelected())
		{
			ImGui::Text("No Entity Selected");
			return;
		}
		flecs::entity selected = Editor::Get().GetEntitySelected();
		if (selected.has<TransformComponent>())
		{
			RenderWithHeader(selected, "Transform");
		}
	}
}
