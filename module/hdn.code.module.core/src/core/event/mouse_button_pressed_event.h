#pragma once

#include "mouse_button_event.h"

namespace hdn
{
	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent( MouseButton button ) : MouseButtonEvent( button ) {}
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << Underlying(GetMouseButton());
			return ss.str();
		}
		EVENT_CLASS_TYPE( EventType::MouseButtonPressed )
	};
}