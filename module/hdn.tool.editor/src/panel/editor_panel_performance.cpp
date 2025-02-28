#include "editor_panel_performance.h"

namespace hdn
{
	void PerformancePanel::OnUpdate(f32 dt)
	{
		ImGui::Text("dt: %.4f", dt * 1000);
	}
}
