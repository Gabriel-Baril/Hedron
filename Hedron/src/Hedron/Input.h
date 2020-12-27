#pragma once

#include "Hedron/Core.h"

namespace Hedron
{
	class HEDRON_API Input
	{
	public:
		inline static bool is_key_pressed(int keyCode) { return s_instance->is_key_pressed_impl(keyCode); }
		//static bool is_key_released(int keyCode) { return s_instance->is_key_pressed_impl(keyCode); }
		inline static bool is_mouse_button_pressed(int mouseButton) { return s_instance->is_mouse_button_pressed_impl(mouseButton); }
		inline static std::pair<float, float> get_mouse_position() { return s_instance->get_mouse_position_impl(); }
		inline static float get_mouse_x() { return s_instance->get_mouse_x_impl(); }
		inline static float get_mouse_y() { return s_instance->get_mouse_x_impl(); }

	protected:
		virtual bool is_key_pressed_impl(int keyCode) = 0;
		virtual bool is_mouse_button_pressed_impl(int mouseButton) = 0;
		virtual std::pair<float, float> get_mouse_position_impl() = 0;
		virtual float get_mouse_x_impl() = 0;
		virtual float get_mouse_y_impl() = 0;
	private:
		static Input* s_instance;
	};
}