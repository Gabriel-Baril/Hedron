#pragma once
#include "KeyEvent.h"

namespace Hedron
{
	class HEDRON_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeatCount) : KeyEvent(keyCode), m_repeatCount(repeatCount) {}

		inline int get_repeat_count() const { return m_repeatCount; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KEY_PRESSED);
	private:
		int m_repeatCount;
	};
}