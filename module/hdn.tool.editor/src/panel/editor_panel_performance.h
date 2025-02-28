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

		void OnUpdate(f32 dt);
	};
}