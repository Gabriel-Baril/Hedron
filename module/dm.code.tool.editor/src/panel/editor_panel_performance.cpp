#include "editor_panel_performance.h"

namespace dm
{
	void PerformancePanel::on_update(f32 dt)
	{
		ImGui::Text("dt: %.4f", dt * 1000);
	}
}
