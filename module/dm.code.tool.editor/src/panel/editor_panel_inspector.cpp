#include "editor_panel_inspector.h"
#include "editor.h"
#include "ecs/components/transform_component.h"
#include "ecs/components/outline_renderer.h"

namespace dm
{
	void InspectorPanel::on_update(f32 dt)
	{
		if (!Editor::get().has_entity_selected())
		{
			ImGui::Text("No Entity Selected");
			return;
		}
		flecs::entity selected = Editor::get().get_entity_selected();
		if (selected.has<TransformComponent>())
		{
			render_with_header(selected, "Transform");
		}
	}
}
