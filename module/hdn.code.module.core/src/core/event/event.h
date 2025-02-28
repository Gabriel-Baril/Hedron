#pragma once

#include "core/core.h"

#define EVENT_CLASS_TYPE(type)  static EventType GetStaticType() { return type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) virtual utype<EventCategory> GetCategoryFlags() const override { return Underlying(category); }

namespace hdn
{
	enum class EventType : u8
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMove, MouseScrolled,
		SceneInit, SceneStart, SceneUpdate, SceneEnd, ScenePause, AceneUnpause
	};

	enum class EventCategory : u8
	{
		None = 0,
		Application = BIT( 0 ),
		Input = BIT( 1 ),
		Keyboard = BIT( 2 ),
		Mouse = BIT( 3 ),
		MouseButton = BIT( 4 )
	};
	ENABLE_ENUM_CLASS_BITWISE_OPERATIONS(EventCategory)

	class Event
	{
		friend class EventDispatcher;
	public:
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual const char *GetName() const = 0;
		virtual utype<EventCategory> GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); } // Has to be overrided if you want more specific details about the event

		inline bool IsInCategory( EventCategory category )
		{
			return GetCategoryFlags() & Underlying( category );
		}
		bool Handled = false;
	};

	inline std::ostream &operator<<( std::ostream &os, const Event &e )
	{
		return os << e.ToString();
	}
}