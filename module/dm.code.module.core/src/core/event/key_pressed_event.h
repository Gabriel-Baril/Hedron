#pragma once

#include "key_event.h"

namespace dm
{
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keyCode, u32 repeatCount) : KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		inline u32 get_repeat_count() const { return m_RepeatCount; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::KEY_PRESSED)
	private:
		u32 m_RepeatCount;
	};
}
