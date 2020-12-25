#pragma once

#include "Event.h"

// TODO: Refactor file layout to create one (.h, .cpp) file per class
// TODO: Create documentation for all of those classes
namespace Hedron
{
	class HEDRON_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}

		inline uint32_t get_width() const { return m_width; }
		inline uint32_t get_height() const { return m_height; }

		std::string to_string()
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

	class HEDRON_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}
		EVENT_CLASS_TYPE(WINDOW_CLOSE)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
	};

	class HEDRON_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(APP_TICK)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
	};

	class HEDRON_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(APP_UPDATE)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
	};

	class HEDRON_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(APP_RENDER)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
	};
}