#pragma once

#include <sstream>
#include "Hedron/Events/Event.h"

namespace Hedron
{
	class HEDRON_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {}

		inline float get_x() const { return m_mouseX; }
		inline float get_y() const { return m_mouseY; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: (" << get_x() << ", " << get_y() << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_MOVE);
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT);
	private:
		float m_mouseX;
		float m_mouseY;
	};
}