#pragma once

#include "imgui.h"
#include "ideation_parser.h"

#include "core/core.h"
#include "core/stl/vector.h"

namespace hdn
{
	class IdaesImgui
	{
	public:
		void Init();
		void Draw();
	private:
		IdeationParser m_Parser;
		vector<Ideation> m_Ideations;
	};
}