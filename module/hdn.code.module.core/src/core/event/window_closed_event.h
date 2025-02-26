#pragma once

#include "event.h"

namespace hdn
{
	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() = default;

		EVENT_CLASS_TYPE( EventType::WindowClose )
		EVENT_CLASS_CATEGORY( EventCategory::Application )
	};
}