#pragma once

#include "Hedron/Core/Core.h"
#include "Hedron/Events/Event.h"

namespace Hedron
{
	struct WindowProps
	{
		std::string title;
		uint32_t width;
		uint32_t height;
		
		WindowProps(const std::string& title = "Hedron Engine",
			uint32_t width = 1600,
			uint32_t height = 900)
			: title(title), width(width), height(height) {}
	};

	// Interface representing a desktop system based window
	class HEDRON_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void on_update() = 0;

		virtual uint32_t get_width() const = 0;
		virtual uint32_t get_height() const = 0;

		// Window attribute
		virtual void set_event_callback(const EventCallbackFn& callback) = 0;
		virtual void set_v_sync(bool enabled) = 0;
		virtual bool is_v_sync() const = 0;

		virtual void* get_native_window() = 0;

		static Scope<Window> create(const WindowProps& props = WindowProps()); // Has to be implemented in all the subclasses of Window
	};
}