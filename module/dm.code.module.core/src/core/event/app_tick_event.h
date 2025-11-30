#pragma once

#include "event.h"

namespace dm
{
	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(EventType::APP_UPDATE)
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION)
	};
}
