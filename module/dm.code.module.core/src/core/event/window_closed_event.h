#pragma once

#include "event.h"

namespace hdn
{
	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() = default;

		EVENT_CLASS_TYPE( EventType::WINDOW_CLOSE )
		EVENT_CLASS_CATEGORY( EventCategory::APPLICATION )
	};
}