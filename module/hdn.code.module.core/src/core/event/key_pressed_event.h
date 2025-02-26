#pragma once

#include "key_event.h"

namespace hdn
{
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent( KeyCode keyCode, u32 repeatCount ) : KeyEvent( keyCode ), m_RepeatCount( repeatCount ) {}

		inline u32 GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE( EventType::KeyPressed )
	private:
		u32 m_RepeatCount;
	};
}