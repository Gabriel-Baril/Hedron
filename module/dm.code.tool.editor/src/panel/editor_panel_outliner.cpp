#include "editor_panel_outliner.h"
#include "core/stl/unordered_map.h"

#include "ecs/components/transform_component.h"
#include "imgui.h"

#include <iostream>
#include "editor.h"

namespace dm
{
	std::unordered_map<std::string, const char *> componentIcons = {
			{"TransformComponent", "📌"},
			{"ModelComponent", "🖼️"},
			{"NativeScriptComponent", "📜"},
			{"PointLightComponent", "💡"},
			{"Camera", "📷"}};

	// Function to get an icon for a component
	const char *get_component_icon(const std::string &componentName)
	{
		return componentIcons.contains(componentName) ? componentIcons[componentName] : "❓";
	}

	void OutlinerPanel::draw_entity_tree(flecs::entity entity, flecs::world &ecs)
	{
		if (!entity.is_valid())
			return;
		ImGui::TableNextRow();

		ImGui::TableNextColumn();
		// Check if the entity has children
		bool hasChildren = false;
		entity.children([&](flecs::entity)
										{ hasChildren = true; });

		ImGuiTreeNodeFlags flags = hasChildren ? 0 : ImGuiTreeNodeFlags_Leaf;
		ImGui::PushID(entity.id());
		bool open = false;
		if (!str_equals(entity.name().c_str(), ""))
		{
			open = ImGui::TreeNodeEx(entity.name().c_str(), flags);
		}
		else
		{
			open = ImGui::TreeNodeEx("<untitled>", flags);
		}

		if (open)
		{
			if (ImGui::IsItemClicked())
			{
				Editor::get().set_entity_selected(entity);
			}
		}

		ImGui::PopID();

		ImGui::TableNextColumn();
		// Display components as icons
		entity.each([&](flecs::id id)
								{
			if (!id.is_pair()) {
				flecs::entity comp = id.entity();
				ImGui::Text("%s", comp.name().c_str());
				ImGui::SameLine();
			} });

		// Recursively draw child entities
		if (open)
		{
			entity.children([&](flecs::entity child)
											{ draw_entity_tree(child, ecs); });
			ImGui::TreePop();
		}
	}

	void OutlinerPanel::on_update(f32 dt)
	{
		if (ImGui::BeginTable("EntityTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Entity", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Components", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableHeadersRow();

			Ref<Scene> scene = Editor::get().get_active_scene();

			auto query = scene->World()->query<TransformComponent>();
			query.each([&](flecs::entity e, TransformComponent &transformC)
								 {
				if (!e.parent().is_valid()) {  // Only render root entities
					draw_entity_tree(e, *scene->World());
				} });
			ImGui::EndTable();
		}
	}
}
