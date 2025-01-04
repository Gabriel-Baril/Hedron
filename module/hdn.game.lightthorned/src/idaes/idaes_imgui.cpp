#include "idaes_imgui.h"

#include "core/core_filesystem.h"

namespace hdn
{
	void IdaesImgui::Init()
	{
		TVector<fspath> ideationFiles = FileSystem::Walk("ideations");

		for (const auto& ideationFile : ideationFiles)
		{
			Ideation ideation;
			HDEBUG("Parsing: '{0}'", ideationFile.string().c_str());
			m_Parser.ParseIdeationFile(ideationFile.string(), ideation);
			m_Ideations.emplace_back(ideation);
		}
	}

	void IdaesImgui::Draw()
	{
		const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

		const ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
		const ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAllColumns;

		ImGui::Begin("IdaesUI");
		if (ImGui::BeginTable("idaes", 3, flags))
		{
			ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_NoHide);
			ImGui::TableSetupColumn("NodeType", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
			ImGui::TableHeadersRow();

			int i = 0;
			for (const auto& ideation : m_Ideations) // TODO: Plug ideation vector
			{
				// if (i == 100)
				// {
				// 	break;
				// }
				i++;
				ImGui::TableNextRow();

				ImGui::TableNextColumn();
				ImGui::Text(ideation.ID.c_str());
				ImGui::TableNextColumn();
				ImGui::Text(ideation.nodeType.c_str());
				ImGui::TableNextColumn();
				ImGui::Text(ideation.name.c_str());
			}
			HINFO("eLEMENT cOUNT: {0}", i);
			ImGui::EndTable();
		}
		ImGui::End();
	}

}
