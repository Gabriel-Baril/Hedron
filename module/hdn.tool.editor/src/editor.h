#pragma once

#include "flecs/flecs.h"

namespace hdn
{
	class Editor
	{
	public:
		static Editor& Get();
		bool HasEntitySelected();
		void SetEntitySelected(flecs::entity entity);
		flecs::entity GetEntitySelected();
	private:
		flecs::entity m_SelectedEntity = flecs::entity::null();
	};
}