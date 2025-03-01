#pragma once

#include "core/core.h"
#include "scene.h"
#include "entity.h"

namespace hdn
{
	class Editor
	{
	public:
		static Editor& Get();

		void SetActiveScene(const Ref<Scene>& scene);
		Ref<Scene> GetActiveScene();

		bool HasEntitySelected();
		void SetEntitySelected(flecs::entity entity);
		flecs::entity GetEntitySelected();
	private:
		flecs::entity m_SelectedEntity = flecs::entity::null(); // Convert to Entity
		Ref<Scene> m_ActiveScene = nullptr;
	};
}