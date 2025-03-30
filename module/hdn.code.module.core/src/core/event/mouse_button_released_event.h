#pragma once

#include "mouse_button_event.h"

namespace hdn
{
	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent( MouseButton button ) : MouseButtonEvent( button ) {}
		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << underlying(get_mouse_button());
			return ss.str();
		}
		EVENT_CLASS_TYPE( EventType::MOUSE_BUTTON_RELEASED )
	};
}