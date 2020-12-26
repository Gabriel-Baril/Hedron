#pragma once

#include "MouseButtonEvent.h"

namespace Hedron
{
	class HEDRON_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << get_mouse_button();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASED)
	};
}