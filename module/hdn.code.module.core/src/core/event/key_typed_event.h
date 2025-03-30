#pragma once

#include "key_event.h"

namespace hdn
{
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent( KeyCode keyCode ) : KeyEvent( keyCode ) {}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE( EventType::KEY_TYPED )
	};
}