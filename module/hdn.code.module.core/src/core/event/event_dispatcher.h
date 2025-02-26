#pragma once

#include "event.h"

namespace hdn
{
	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool( T & )>;
	public:
		EventDispatcher( Event &event ) : m_Event( event ) {}

		template<typename T>
		bool Dispatch( EventFn<T> func )
		{
			// Check if the template parameter matchs the current event type
			if ( m_Event.GetEventType() == T::GetStaticType() )
			{
				m_Event.Handled = func( *(T *)&m_Event );
				return true;
			}
			return false;
		}

	private:
		Event &m_Event;
	};
}