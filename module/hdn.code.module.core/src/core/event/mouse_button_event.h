#pragma once

#include "event.h"
#include "mouse_button_code.h"

namespace hdn
{
	class MouseButtonEvent : public Event
	{
	public:
		inline MouseButton GetMouseButton() const { return m_Button; }
		inline virtual std::string ToString() const = 0;
		EVENT_CLASS_CATEGORY( EventCategory::Mouse | EventCategory::Input )
	protected:
		MouseButtonEvent( MouseButton button ) : m_Button( button ) {}
	protected:
		MouseButton m_Button;
	};
}