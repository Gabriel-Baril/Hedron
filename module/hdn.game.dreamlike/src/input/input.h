#pragma once

#include "core/event/key_code.h"
#include "core/event/mouse_button_code.h"

namespace hdn
{
	class Input
	{
	public:
		static bool GetKey(KeyCode keyCode);
		static bool GetMouseButton(MouseButton mouseButton);

		static vec2f32 GetMousePosition();
	};
}