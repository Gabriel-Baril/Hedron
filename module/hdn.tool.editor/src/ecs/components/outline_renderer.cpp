#include "outline_renderer.h"
#include "imgui.h"
#include "core/profiler/profiler.h"

namespace hdn
{
	static const ImVec4 BUTTON_COLOR_X = ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f };
	static const ImVec4 BUTTON_COLOR_X_HOVERED = ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f };
	static const ImVec4 BUTTON_COLOR_X_ACTIVE = ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f };

	static const ImVec4 BUTTON_COLOR_Y = ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f };
	static const ImVec4 BUTTON_COLOR_Y_HOVERED = ImVec4{ 0.3f, 0.8f, 0.2f, 1.0f };
	static const ImVec4 BUTTON_COLOR_Y_ACTIVE = ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f };

	static const ImVec4 BUTTON_COLOR_Z = ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f };
	static const ImVec4 BUTTON_COLOR_Z_HOVERED = ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f };
	static const ImVec4 BUTTON_COLOR_Z_ACTIVE = ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f };

	static void RenderVecEntry(float width, const char* resetLabelName, const char* dragLabelID, float* valueToChange, bool* valueChanged, float resetValue, ImFont* boldFont, ImVec2 buttonSize, ImVec4 buttonColor, ImVec4 hoveredButtonColor, ImVec4 activeButtonColor)
	{
		ImGui::PushItemWidth(width);
		ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoveredButtonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeButtonColor);

		ImGui::PushFont(boldFont);

		if (ImGui::Button(resetLabelName, buttonSize))
		{
			HINFO("Value Changed: {0}", *valueChanged);
			if (valueChanged != nullptr && !(*valueChanged))
			{
				HINFO("IN");
				*valueChanged = true;
				*valueToChange = resetValue;
			}
		}
		ImGui::PopFont();

		ImGui::SameLine();

		bool changed = ImGui::DragFloat(dragLabelID, valueToChange, 0.1f, 0.0f, 0.0f, "%.2f");
		if (valueChanged != nullptr && !(*valueChanged))
			*valueChanged = changed;

		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);
	}

	static void Render(const char* labelName, vec3f32& values, bool* valueChanged, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(labelName);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(labelName);
		ImGui::NextColumn();

		float individual_width = ImGui::CalcItemWidth() / 2.5f; // Divide for each item
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.0f, 0.0f });

		float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		RenderVecEntry(individual_width, "X", "##X", &values.x, valueChanged, resetValue, boldFont, buttonSize, BUTTON_COLOR_X, BUTTON_COLOR_X_HOVERED, BUTTON_COLOR_X_ACTIVE);
		RenderVecEntry(individual_width, "Y", "##Y", &values.y, valueChanged, resetValue, boldFont, buttonSize, BUTTON_COLOR_Y, BUTTON_COLOR_Y_HOVERED, BUTTON_COLOR_Y_ACTIVE);
		RenderVecEntry(individual_width, "Z", "##Z", &values.z, valueChanged, resetValue, boldFont, buttonSize, BUTTON_COLOR_Z, BUTTON_COLOR_Z_HOVERED, BUTTON_COLOR_Z_ACTIVE);

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	void inspector_component_render(TransformComponent& component)
	{
		// HDR_PROFILE_FUNCTION();
		bool changed = false;
		vec3f32 translation = component.position;
		Render("Translation", translation, &component.HasTranslationChanged);
		if (component.HasTranslationChanged)
		{
			component.position = translation;
			changed = true;
		}

		vec3f32 rotation = component.rotation;
		Render("Rotation", rotation, &component.HasRotationChanged);
		if (component.HasRotationChanged)
		{
			component.rotation = rotation;
			changed = true;
		}

		vec3f32 scale = component.scale;
		Render("Scale", scale, &component.HasScaleChanged, 1.0f);
		if (component.HasScaleChanged)
		{
			component.scale = scale;
			changed = true;
		}

		if (changed)
		{
			component.worldMatrix = component.Mat4();
		}
	}

	void RenderWithHeader(flecs::entity& entity, const char* controlLabel)
	{
		using Component = TransformComponent; // TODO: Should be a template paramter in the futur

		if (!entity.has<Component>()) return;

		const ImGuiTreeNodeFlags tree_node_flags =
			ImGuiTreeNodeFlags_DefaultOpen |
			ImGuiTreeNodeFlags_AllowItemOverlap |
			ImGuiTreeNodeFlags_SpanAvailWidth |
			ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_FramePadding;
		Component& component = *entity.get_mut<Component>();
		ImVec2 content_region_available = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });

		float line_height = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;

		ImGui::Separator();

		ImGui::Columns(3, nullptr, false);

		float main_column_width = content_region_available.x - 50.0f;
		float child_column_width = 0.5f * (content_region_available.x - main_column_width);
		ImGui::SetColumnWidth(0, main_column_width);
		ImGui::SetColumnWidth(1, child_column_width);
		ImGui::SetColumnWidth(2, child_column_width);

		bool is_opened = ImGui::TreeNodeEx((void*)typeid(Component).hash_code(), tree_node_flags, controlLabel);

		ImGui::NextColumn();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.0f);
		if (ImGui::Button("+", ImVec2{ line_height, line_height }))
			ImGui::OpenPopup("ComponentSettings");

		ImGui::NextColumn();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.0f);
		ImGui::Checkbox("Enabled", &component.enabled);

		ImGui::Columns(1);
		ImGui::PopStyleVar();

		bool remove_component = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove Component"))
			{
				remove_component = true;
			}

			const char* enable_label = component.enabled ? "Disable" : "Enable";
			if (ImGui::MenuItem(enable_label))
			{
				component.enabled = !component.enabled;
			}

			ImGui::EndPopup();
		}

		if (is_opened)
		{
			inspector_component_render(component);

			ImGui::TreePop();
		}

		if (remove_component)
		{
			entity.remove<Component>();
		}
	}
}
