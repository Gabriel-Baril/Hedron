#pragma once

#include "event.h"

namespace hdn
{
	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE( EventType::AppUpdate )
		EVENT_CLASS_CATEGORY( EventCategory::Application )
	};
}