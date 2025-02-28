#pragma once

#include "imgui.h"
#include "ideation_parser.h"

#include "core/core.h"
#include "core/stl/vector.h"

#include "editor_panel.h"

namespace hdn
{
	class IdeationManagerPanel : public IEditorPanel
	{
	public:
		IdeationManagerPanel()
			: IEditorPanel{ "ideation manager" }
		{
		}

		void Init();
		void OnUpdate(f32 dt);
	private:
		IdeationParser m_Parser;
		vector<Ideation> m_Ideations;
		bool m_Initialized = false;
	};
}