#include "editor_panel_ideation_manager.h"

#include "core/stl/vector.h"
#include "core/core_filesystem.h"

namespace dm
{
	void IdeationManagerPanel::init()
	{
		vector<fspath> ideationFiles = filesystem_walk("ideations");

		for (const auto &ideationFile : ideationFiles)
		{
			Ideation ideation;
			HDEBUG("Parsing: '{0}'", ideationFile.string().c_str());
			m_Parser.parse_ideation_file(ideationFile.string(), ideation);
			m_Ideations.emplace_back(ideation);
		}
	}

	void IdeationManagerPanel::on_update(f32 dt)
	{
		if (!m_Initialized)
		{
			init();
			m_Initialized = true;
		}

		const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;

		const ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

		if (ImGui::BeginTable("idaes", 3, flags))
		{
			ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_NoHide);
			ImGui::TableSetupColumn("NodeType", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
			ImGui::TableHeadersRow();

			int i = 0;
			for (const auto &ideation : m_Ideations)
			{
				i++;
				ImGui::TableNextRow();

				ImGui::TableNextColumn();
				ImGui::Text(ideation.ID.c_str());
				ImGui::TableNextColumn();
				ImGui::Text(ideation.nodeType.c_str());
				ImGui::TableNextColumn();
				ImGui::Text(ideation.name.c_str());
			}
			// HDEBUG("eLEMENT cOUNT: {0}", i);
			ImGui::EndTable();
		}
	}
}
