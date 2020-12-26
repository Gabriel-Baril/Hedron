#pragma once

#include "Event.h"

namespace Hedron
{
	class HEDRON_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {}

		inline float get_x_offset() const { return m_xOffset; }
		inline float get_y_offset() const { return m_yOffset; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: (" << get_x_offset() << ", " << get_y_offset() << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_SCROLLED);
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT);
	private:
		float m_xOffset;
		float m_yOffset;
	};
}