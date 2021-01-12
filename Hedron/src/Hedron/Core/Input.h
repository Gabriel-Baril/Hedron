#pragma once

#include "Hedron/Core/Core.h"

namespace Hedron
{
	class HEDRON_API Input
	{
	public:
		static bool is_key_pressed(int keyCode);
		static bool is_mouse_button_pressed(int mouseButton);

		static std::pair<float, float> get_mouse_position();
		static float get_mouse_x();
		static float get_mouse_y();
	};
}