
#pragma once

#include "HDNWindow.h"

namespace hdn
{
	class FirstApp
	{
	public:
		static constexpr uint32 WIDTH = 800;
		static constexpr uint32 HEIGHT = 600;
	public:
		FirstApp();
		void Run();
	private:
		HDNWindow m_Window;
	};
}