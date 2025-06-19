#pragma once

#include "imgui.h"
#include "core/core.h"

namespace hdn
{
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