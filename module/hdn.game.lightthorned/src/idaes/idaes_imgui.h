#pragma once

#include "imgui.h"
#include "ideation_parser.h"
#include "core/core.h"

namespace hdn
{
	class IdaesImgui
	{
	public:
		void Init();
		void Draw();
	private:
		IdeationParser m_Parser;
		TVector<Ideation> m_Ideations;
	};
}