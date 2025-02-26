#pragma once

#include "event.h"
#include "key_code.h"

namespace hdn
{
	class KeyEvent : public Event
	{
	public:
		inline KeyCode GetKeyCode() const { return m_KeyCode; }
		inline virtual std::string ToString() const = 0;

		EVENT_CLASS_CATEGORY( EventCategory::Keyboard | EventCategory::Input )
	protected:
		KeyEvent( KeyCode keyCode ) : m_KeyCode( keyCode ) {}
	protected:
		KeyCode m_KeyCode;
	};
}