#include "editor.h"

namespace hdn
{
	Editor& Editor::Get()
	{
		static Editor s_Instance{};
		return s_Instance;
	}

	bool Editor::HasEntitySelected()
	{
		return m_SelectedEntity != flecs::entity::null();
	}

	void Editor::SetEntitySelected(flecs::entity entity)
	{
		m_SelectedEntity = entity;
	}

	flecs::entity Editor::GetEntitySelected()
	{
		return m_SelectedEntity;
	}
}
