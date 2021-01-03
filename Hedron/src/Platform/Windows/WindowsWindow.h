#pragma once

#include "Hedron/Core/Window.h"
#include "Hedron/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Hedron
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		virtual void on_update() override;

		inline virtual unsigned int get_width() const override { return m_data.width; };
		inline virtual unsigned int get_height() const override { return m_data.height; };

		// Window attributes
		inline virtual void set_event_callback(const EventCallbackFn& callback) override { m_data.eventCallback = callback; }
		virtual void set_v_sync(bool enabled) override;
		virtual bool is_v_sync() const override;

		inline virtual void* get_native_window() override { return static_cast<void*>(m_window); }
	private:
		void set_window_resize_callback();
		void set_window_close_callback();
		void set_keyboard_callback();
		void set_char_callback();
		void set_mouse_button_callback();
		void set_scroll_callback();
		void set_mouse_moved_callback();

		virtual void init(const WindowProps& props);
		virtual void shutdown();
	private:
		GLFWwindow* m_window;
		GraphicsContext* m_context;


		// Custom user data to GLFW
		struct WindowData
		{
			std::string title;
			unsigned int width;
			unsigned int height;
			bool vSync;

			EventCallbackFn eventCallback;
		};

		WindowData m_data;
	};
}