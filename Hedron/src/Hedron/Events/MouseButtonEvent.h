#pragma once

#include "Hedron/Events/Event.h"

namespace Hedron
{
	class HEDRON_API MouseButtonEvent : public Event
	{
	public:
		inline int get_mouse_button() const { return m_button; }
		inline virtual std::string to_string() const = 0;

		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT);
	protected:
		MouseButtonEvent(int button) : m_button(button) {}
		int m_button;
	};
}