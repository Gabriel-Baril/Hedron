#pragma once

#include "hdrpch.h"

#include "Hedron/Core.h"

namespace Hedron
{
	// Events are currently blocking, in the future we might want to do an event bus

	enum class EventType
	{
		NONE = 0,
		WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_FOCUS, WINDOW_LOST_FOCUS, WINDOW_MOVED,
		APP_TICK, APP_UPDATE, APP_RENDER,
		KEY_PRESSED, KEY_RELEASED,
		MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_MOVE, MOUSE_SCROLLED
	};

	enum EventCategory
	{
		NONE = 0,
		EVENT_CATEGORY_APPLICATION  = BIT(0),
		EVENT_CATEGORY_INPUT        = BIT(1),
		EVENT_CATEGORY_KEYBOARD     = BIT(2),
		EVENT_CATEGORY_MOUSE        = BIT(3),
		EVENT_CATEGORY_MOUSE_BUTTON = BIT(4)
	};


#define EVENT_CLASS_TYPE(type)  static EventType get_static_type() { return EventType::##type; }\
								virtual EventType get_event_type() const override { return get_static_type(); }\
								virtual const char* get_name() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int get_category_flags() const override { return category; }

	class HEDRON_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType get_event_type() const = 0;
		virtual const char* get_name() const = 0;
		virtual int get_category_flags() const = 0;
		virtual std::string to_string() const { return get_name(); } // Has to be overrided if you want more specific details about the event
		virtual void set_handled(bool handled) { this->m_handled = handled; }
		virtual bool is_handled() { return m_handled; }

		inline bool is_in_category(EventCategory category)
		{
			return get_category_flags() & category;
		}
	protected:
		bool m_handled = false;
	};

	// Used to dispatch events based on their type
	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) : m_event(event) {}

		template<typename T>
		bool dispatch(EventFn<T> func)
		{
			// Check if the template parameter matchs the current event type
			if (m_event.get_event_type() == T::get_static_type())
			{
				m_event.m_handled = func(*(T*)&m_event);
				return true;
			}
			return false;
		}

	private:
		Event& m_event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.to_string();
	}
}