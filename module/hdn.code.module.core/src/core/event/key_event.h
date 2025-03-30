#pragma once

#include "event.h"
#include "key_code.h"

namespace hdn
{
	class KeyEvent : public Event
	{
	public:
		inline KeyCode get_key_code() const { return m_KeyCode; }
		inline virtual std::string to_string() const = 0;

		EVENT_CLASS_CATEGORY( EventCategory::KEYBOARD | EventCategory::INPUT )
	protected:
		KeyEvent( KeyCode keyCode ) : m_KeyCode( keyCode ) {}
	protected:
		KeyCode m_KeyCode;
	};
}