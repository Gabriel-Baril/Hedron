#pragma once

#include "core/core.h"

#include "editor_panel.h"

namespace hdn
{
	class PerformancePanel : public IEditorPanel
	{
	public:
		PerformancePanel()
			: IEditorPanel{ "performance" }
		{
		}

		void on_update(f32 dt);
	};
}