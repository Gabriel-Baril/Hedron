#pragma once

#include "key_event.h"

namespace hdn
{
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent( KeyCode keyCode ) : KeyEvent( keyCode ) {}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE( EventType::KEY_RELEASED )
	};
}