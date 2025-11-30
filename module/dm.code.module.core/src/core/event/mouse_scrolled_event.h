#pragma once

#include "event.h"

namespace dm
{
	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(f32 x_offset, f32 y_offset) : m_OffsetX(x_offset), m_OffsetY(y_offset) {}

		inline f32 get_offset_x() const { return m_OffsetX; }
		inline f32 get_offset_y() const { return m_OffsetY; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: (" << get_offset_x() << ", " << get_offset_y() << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::MOUSE_SCROLLED)
		EVENT_CLASS_CATEGORY(EventCategory::MOUSE | EventCategory::INPUT)
	private:
		f32 m_OffsetX;
		f32 m_OffsetY;
	};
}
