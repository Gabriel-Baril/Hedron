#pragma once

#include "core/core.h"
#include "flecs/flecs.h"
#include "editor_panel.h"

namespace dm
{
	class InspectorPanel : public IEditorPanel
	{
	public:
		InspectorPanel()
				: IEditorPanel{"inspector"}
		{
		}

		void on_update(f32 dt);

	private:
	};
}
