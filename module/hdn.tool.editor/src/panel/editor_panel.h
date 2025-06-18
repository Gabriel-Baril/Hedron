#pragma once

#include "imgui.h"
#include "core/core.h"

namespace hdn
{
	// void editor_f32_field( float *v );
	// void editor_vec2f32_field( vec2f32 *v );
	// void editor_vec3f32_field( vec3f32 *v );
	// void editor_vec4f32_field( vec4f32 *v );
	// void editor_checkbox( bool *checked );
	// void editor_begin_panel();
	// void editor_end_panel();

	class IEditorPanel
	{
	public:
		IEditorPanel(const string name)
			: m_Name{name}
		{
		}

		virtual void begin()
		{
			ImGui::Begin(m_Name.c_str());
		}
		virtual void on_update( f32 dt ) = 0;
		virtual void end()
		{
			ImGui::End();
		}

		void enable()
		{
			m_Enabled = true;
		}

		void disable()
		{
			m_Enabled = false;
		}

		bool enabled()
		{
			return m_Enabled;
		}
	private:
		bool m_Enabled = true;
		string m_Name;
	};
}