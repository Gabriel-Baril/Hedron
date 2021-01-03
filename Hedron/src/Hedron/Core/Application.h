#pragma once

#include "Hedron/Core/Core.h"
#include "Hedron/Core/Window.h"
#include "Hedron/Core/LayerStack.h"
#include "Hedron/Core/Timestep.h"

#include "Hedron/Events/Event.h"
#include "Hedron/Events/ApplicationEvents.h"

#include "Hedron/ImGui/ImGuiLayer.h"

namespace Hedron
{
	class HEDRON_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
		void on_event(Event& e);

		void push_layer(Layer* layer);
		void push_overlay(Layer* overlay);

		inline static Application& get() { return *s_instance; }
		inline Window& get_window() { return *m_window; };
	private:
		bool on_window_close(WindowCloseEvent& windowCloseEvent);
		bool on_window_resize(WindowResizeEvent& windowResizeEvent);
	private:
		std::unique_ptr<Window> m_window;
		ImGuiLayer* m_imGuiLayer;
		bool m_running = true;
		bool m_minimised = false;
		LayerStack m_layerStack;
		float m_lastFrameTime = 0;
	private:
		static Application* s_instance;
	};

	// To be defined in the client
	Application* create_application();
}

