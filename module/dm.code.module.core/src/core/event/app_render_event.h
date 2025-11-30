#pragma once

#include "event.h"

namespace dm
{
	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(EventType::APP_UPDATE)
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION)
	};
}
