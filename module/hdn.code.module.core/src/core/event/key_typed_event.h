#pragma once

#include "key_event.h"

namespace hdn
{
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent( KeyCode keyCode ) : KeyEvent( keyCode ) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE( EventType::KeyTyped )
	};
}