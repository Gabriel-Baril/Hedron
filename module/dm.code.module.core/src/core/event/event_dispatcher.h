#pragma once

#include "event.h"

namespace dm
{
	class EventDispatcher
	{
		template <typename T>
		using EventFn = std::function<bool(T &)>;

	public:
		EventDispatcher(Event &event) : m_Event(event) {}

		template <typename T>
		bool dispatch(EventFn<T> func)
		{
			// Check if the template parameter matchs the current event type
			if (m_Event.get_event_type() == T::get_static_type())
			{
				m_Event.Handled = func(*(T *)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event &m_Event;
	};
}
