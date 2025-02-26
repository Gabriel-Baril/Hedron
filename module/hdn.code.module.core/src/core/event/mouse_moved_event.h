#pragma once

#include "event.h"

namespace hdn
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent( f32 x, f32 y ) : m_MouseX( x ), m_MouseY( y ) {}

		inline f32 GetMouseX() const { return m_MouseX; }
		inline f32 GetMouseY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: (" << GetMouseX() << ", " << GetMouseY() << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE( EventType::MouseMove )
		EVENT_CLASS_CATEGORY( EventCategory::Mouse | EventCategory::Input )
	private:
		f32 m_MouseX;
		f32 m_MouseY;
	};
}