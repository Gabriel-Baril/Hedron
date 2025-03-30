#pragma once

#include "event.h"

namespace hdn
{
	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE( EventType::APP_UPDATE )
		EVENT_CLASS_CATEGORY( EventCategory::APPLICATION )
	};
}