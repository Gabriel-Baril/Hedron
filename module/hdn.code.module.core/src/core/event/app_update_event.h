#pragma once

#include "event.h"

namespace hdn
{
	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE( EventType::AppUpdate )
		EVENT_CLASS_CATEGORY( EventCategory::Application )
	};
}