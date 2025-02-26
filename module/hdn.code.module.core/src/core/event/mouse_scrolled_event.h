#pragma once

#include "event.h"

namespace hedron
{
	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent( f32 x_offset, f32 y_offset ) : m_OffsetX( x_offset ), m_OffsetY( y_offset ) {}

		inline f32 GetOffsetX() const { return m_OffsetX; }
		inline f32 GetOffsetY() const { return m_OffsetY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: (" << GetOffsetX() << ", " << GetOffsetY() << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE( EventType::MouseScrolled )
		EVENT_CLASS_CATEGORY( EventCategory::Mouse | EventCategory::Input )
	private:
		f32 m_OffsetX;
		f32 m_OffsetY;
	};
}