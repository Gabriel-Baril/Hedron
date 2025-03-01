#include "editor.h"

namespace hdn
{
	Editor& Editor::Get()
	{
		static Editor s_Instance{};
		return s_Instance;
	}

	void Editor::SetActiveScene(const Ref<Scene>& scene)
	{
		m_ActiveScene = scene;
	}

	Ref<Scene> Editor::GetActiveScene()
	{
		return m_ActiveScene;
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
