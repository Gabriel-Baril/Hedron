#pragma once

#include "Event.h"

namespace Hedron
{
	class HEDRON_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(APP_UPDATE)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
	};
}