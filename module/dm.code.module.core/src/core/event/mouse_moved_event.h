#pragma once

#include "event.h"

namespace hdn
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent( f32 x, f32 y ) : m_MouseX( x ), m_MouseY( y ) {}

		inline f32 get_mouse_x() const { return m_MouseX; }
		inline f32 get_mouse_y() const { return m_MouseY; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: (" << get_mouse_x() << ", " << get_mouse_y() << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE( EventType::MOUSE_MOVE )
		EVENT_CLASS_CATEGORY( EventCategory::MOUSE | EventCategory::INPUT )
	private:
		f32 m_MouseX;
		f32 m_MouseY;
	};
}