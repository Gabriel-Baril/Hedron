#pragma once

#include "Core.h"
#include "Window.h"
#include "Hedron/LayerStack.h"
#include "Events/Event.h"
#include "Hedron/Events/ApplicationEvents.h"

#include "Hedron/ImGui/ImGuiLayer.h"

#include "Hedron/Renderer/Shader.h"

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
		bool on_window_closed(WindowCloseEvent& windowCloseEvent);

		std::unique_ptr<Window> m_window;
		ImGuiLayer* m_imGuiLayer;
		bool m_running = true;
		LayerStack m_layerStack;

		unsigned int m_vertex_array, m_vertex_buffer, m_index_buffer;
		std::unique_ptr<Shader> m_shader;
	private:
		static Application* s_instance;
	};

	// To be defined in the client
	Application* create_application();
}

