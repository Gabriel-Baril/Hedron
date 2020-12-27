#pragma once

#include "Hedron/Events/ApplicationEvents.h"
#include "Hedron/Events/KeyboardEvents.h"
#include "Hedron/Events/MouseEvents.h"
#include "Hedron/Layer.h"

namespace Hedron
{
	class HEDRON_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void on_attach();
		void on_detach();
		void on_update();
		void on_event(Event& event);
	private:
		bool on_mouse_button_pressed_event(MouseButtonPressedEvent& e);
		bool on_mouse_button_released_event(MouseButtonReleasedEvent& e);
		bool on_mouse_moved_event(MouseMovedEvent& e);
		bool on_mouse_scrolled_event(MouseScrolledEvent& e);
		bool on_key_released_event(KeyReleasedEvent& e);
		bool on_key_pressed_event(KeyPressedEvent& e);
		bool on_key_typed_event(KeyTypedEvent& e);
		bool on_window_resize_event(WindowResizeEvent& e);

	private:
		float m_time = 0.0f;
	};
}