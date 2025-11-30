#pragma once

#include "mouse_button_event.h"

namespace dm
{
	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(MouseButton button) : MouseButtonEvent(button) {}
		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << underlying(get_mouse_button());
			return ss.str();
		}
		EVENT_CLASS_TYPE(EventType::MOUSE_BUTTON_PRESSED)
	};
}
