#pragma once

#include "event.h"

namespace hdn
{
	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent( u32 width, u32 height ) : m_Width( width ), m_Height( height ) {}

		inline u32 get_width() const { return m_Width; }
		inline u32 get_height() const { return m_Height; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: [width: " << get_width() << ", height: " << get_height() << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::WINDOW_RESIZE)
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION)
	private:
		u32 m_Width;
		u32 m_Height;
	};
}