#pragma once

#include <Hedron/Core/Core.h>
#include <Hedron/Core/Log.h>
#include <Hedron/Scene/Scene.h>
#include <Hedron/Scene/Entity.h>

namespace Hedron
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);
		
		void set_context(const Ref<Scene>& context);
		void on_imgui_render();

		Entity get_selected_entity() const { return m_selectionContext; }
	private:
		void draw_entity_node(Entity entity);
		void draw_components(Entity entity);
	private:
		Ref<Scene> m_context;
		Entity m_selectionContext;
	};
}