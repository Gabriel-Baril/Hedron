#pragma once

#include "event.h"
#include "mouse_button_code.h"

namespace hdn
{
	class MouseButtonEvent : public Event
	{
	public:
		inline MouseButton get_mouse_button() const { return m_Button; }
		inline virtual std::string to_string() const = 0;
		EVENT_CLASS_CATEGORY( EventCategory::MOUSE | EventCategory::INPUT )
	protected:
		MouseButtonEvent( MouseButton button ) : m_Button( button ) {}
	protected:
		MouseButton m_Button;
	};
}