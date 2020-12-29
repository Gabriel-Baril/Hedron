#pragma once

#include "Event.h"

namespace Hedron
{
	class HEDRON_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}

		inline uint32_t get_width() const { return m_width; }
		inline uint32_t get_height() const { return m_height; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: [width: " << get_width() << ", height: " << get_height() << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WINDOW_RESIZE)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION);
	private:
		uint32_t m_width;
		uint32_t m_height;
	};
}