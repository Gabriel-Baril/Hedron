#pragma once

#include "event.h"

namespace hdn
{
	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent( u32 width, u32 height ) : m_Width( width ), m_Height( height ) {}

		inline u32 GetWidth() const { return m_Width; }
		inline u32 GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: [width: " << GetWidth() << ", height: " << GetHeight() << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE( EventType::WindowResize )
			EVENT_CLASS_CATEGORY( EventCategory::Application )
	private:
		u32 m_Width;
		u32 m_Height;
	};
}