#pragma once

#include "mouse_button_event.h"

namespace hdn
{
	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent( MouseButton button ) : MouseButtonEvent( button ) {}
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << Underlying(GetMouseButton());
			return ss.str();
		}
		EVENT_CLASS_TYPE( EventType::MouseButtonReleased )
	};
}