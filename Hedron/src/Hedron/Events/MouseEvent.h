#pragma once

#include "Event.h"

// TODO: Refactor file layout to create one (.h, .cpp) file per class
// TODO: Create documentation for all of those classes
namespace Hedron
{
	class HEDRON_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {}

		inline float get_x() const { return m_mouseX; }
		inline float get_y() const { return m_mouseX; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MosueMovedEvent: (" << get_x() << ", " << get_y() << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_MOVE);
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT);
	private:
		float m_mouseX;
		float m_mouseY;
	};

	class HEDRON_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {}

		inline float get_x_offset() const { return m_xOffset; }
		inline float get_y_offset() const { return m_yOffset; }

		std::string to_string()
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: (" << get_x_offset() << ", " << get_y_offset() << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_SCROLLED);
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT);
	private:
		float m_xOffset;
		float m_yOffset;
	};

	class HEDRON_API MouseButtonEvent : public Event
	{
	public:
		inline int get_mouse_button() const { return m_button; }
		
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT);
	protected:
		MouseButtonEvent(int button) : m_button(button) {}
		int m_button;
	};

	class HEDRON_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

		std::string to_string()
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << get_mouse_button();
		}

		EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESSED)
	};

	class HEDRON_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

		std::string to_string()
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << get_mouse_button();
		}

		EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASED)
	};
}