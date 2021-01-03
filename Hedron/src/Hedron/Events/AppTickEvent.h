#pragma once

#include "Hedron/Events/Event.h"

namespace Hedron
{
	class HEDRON_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(APP_TICK)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
	};
}