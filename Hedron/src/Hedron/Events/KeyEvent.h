#pragma once

#include "Hedron/Events/Event.h"

// TODO: Refactor file layout to create one (.h, .cpp) file per class
// TODO: Create documentation for all of those classes
namespace Hedron
{
	class HEDRON_API KeyEvent : public Event
	{
	public:
		inline int get_key_code() const { return m_keyCode; }
		inline virtual std::string to_string() const = 0;

		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT)
	protected:
		KeyEvent(int keyCode) : m_keyCode(keyCode) {}
		int m_keyCode;
	};
}