#pragma once

#include "core/core.h"
#include "scene.h"
#include "entity.h"

namespace hdn
{
	class Editor
	{
	public:
		static Editor& get();

		void set_active_scene(const Ref<Scene>& scene);
		Ref<Scene> get_active_scene();

		bool has_entity_selected();
		void set_entity_selected(flecs::entity entity);
		flecs::entity get_entity_selected();
	private:
		flecs::entity m_SelectedEntity = flecs::entity::null(); // Convert to Entity
		Ref<Scene> m_ActiveScene = nullptr;
	};
}