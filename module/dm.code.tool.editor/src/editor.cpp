#include "editor.h"

namespace hdn
{
	Editor& Editor::get()
	{
		static Editor s_Instance{};
		return s_Instance;
	}

	void Editor::set_active_scene(const Ref<Scene>& scene)
	{
		m_ActiveScene = scene;
	}

	Ref<Scene> Editor::get_active_scene()
	{
		return m_ActiveScene;
	}

	bool Editor::has_entity_selected()
	{
		return m_SelectedEntity != flecs::entity::null();
	}

	void Editor::set_entity_selected(flecs::entity entity)
	{
		m_SelectedEntity = entity;
	}

	flecs::entity Editor::get_entity_selected()
	{
		return m_SelectedEntity;
	}
}
