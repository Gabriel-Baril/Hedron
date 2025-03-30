#pragma once

#include "core/core.h"

#define EVENT_CLASS_TYPE(type)  static EventType get_static_type() { return type; }\
								virtual EventType get_event_type() const override { return get_static_type(); }\
								virtual const char* get_name() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) virtual utype<EventCategory> get_category_flags() const override { return underlying(category); }

namespace hdn
{
	enum class EventType : u8
	{
		NONE = 0,
		WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_FOCUS, WINDOW_LOST_FOCUS, WINDOW_MOVED,
		APP_TICK, APP_UPDATE, APP_RENDER,
		KEY_PRESSED, KEY_RELEASED, KEY_TYPED,
		MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_MOVE, MOUSE_SCROLLED,
		SCENE_INIT, SCENE_START, SCENE_UPDATE, SCENE_END, SCENE_PAUSE, SCENE_UNPAUSE
	};

	enum class EventCategory : u8
	{
		NONE = 0,
		APPLICATION = BIT( 0 ),
		INPUT = BIT( 1 ),
		KEYBOARD = BIT( 2 ),
		MOUSE = BIT( 3 ),
		MOUSE_BUTTON = BIT( 4 )
	};
	ENABLE_ENUM_CLASS_BITWISE_OPERATIONS(EventCategory)

	class Event
	{
		friend class EventDispatcher;
	public:
		virtual ~Event() = default;

		virtual EventType get_event_type() const = 0;
		virtual const char *get_name() const = 0;
		virtual utype<EventCategory> get_category_flags() const = 0;
		virtual std::string to_string() const { return get_name(); } // Has to be overrided if you want more specific details about the event

		inline bool has_category( EventCategory category )
		{
			return get_category_flags() & underlying( category );
		}
		bool Handled = false;
	};

	inline std::ostream &operator<<( std::ostream &os, const Event &e )
	{
		return os << e.to_string();
	}
}