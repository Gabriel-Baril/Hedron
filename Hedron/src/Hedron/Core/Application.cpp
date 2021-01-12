#include "hdrpch.h"

#include <GLFW/glfw3.h>

#include "Hedron/Core/Application.h"
#include "Hedron/Core/Input.h"

#include "Hedron/Renderer/BufferLayout.h"
#include "Hedron/Renderer/Renderer.h"
#include "Hedron/Renderer/Renderer2D.h"



namespace Hedron
{
	Application* Application::s_instance = nullptr;

	Application::Application(const std::string& name)
		: m_name(name)
	{
		HDR_CORE_ASSERT(!s_instance, "Application already exists!");
		HDR_PROFILE_FUNCTION();

		s_instance = this;

		m_window = Window::create(WindowProps(name));
		m_window->set_event_callback(HDR_BIND_EVENT_FN(Application::on_event));

		Renderer::init();

		m_imGuiLayer = new ImGuiLayer();
		push_overlay(m_imGuiLayer);
	}

	Application::~Application()
	{
		HDR_PROFILE_FUNCTION();

		Renderer::shutdown();
	}

	void Application::on_event(Event& e)
	{
		HDR_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(HDR_BIND_EVENT_FN(Application::on_window_close));
		dispatcher.dispatch<WindowResizeEvent>(HDR_BIND_EVENT_FN(Application::on_window_resize));

		// Maybe replace this with a while loop
		for (auto it = m_layerStack.end(); it != m_layerStack.begin(); )
		{
			if (e.handled)
				break;
			(*--it)->on_event(e);
		}
	}

	void Application::push_layer(Layer* layer)
	{
		HDR_PROFILE_FUNCTION();

		m_layerStack.push_layer(layer);
		layer->on_attach();
	}

	void Application::push_overlay(Layer* overlay)
	{
		HDR_PROFILE_FUNCTION();

		m_layerStack.push_overlay(overlay);
		overlay->on_attach();
	}

	void Application::close()
	{
		m_running = false;
	}

	bool Application::on_window_close(WindowCloseEvent& windowCloseEvent)
	{
		m_running = false;
		return true;
	}

	bool Application::on_window_resize(WindowResizeEvent& windowResizeEvent)
	{
		HDR_PROFILE_FUNCTION();

		if (windowResizeEvent.get_width() == 0 || windowResizeEvent.get_height() == 0)
		{
			m_minimised = true;
			return false;
		}
		m_minimised = false;

		Renderer::on_window_resize(windowResizeEvent.get_width(), windowResizeEvent.get_height());

		return false;
	}


	void Application::run()
	{
		HDR_PROFILE_FUNCTION();

		while (m_running)
		{
			HDR_PROFILE_SCOPE("RUN LOOP | Application::run()");
			float time = (float)glfwGetTime(); // Should be Platform::GetTime() in the future 
			Timestep timestep = time - m_lastFrameTime;
			m_lastFrameTime = time;

			if (!m_minimised)
			{
				{
					HDR_PROFILE_SCOPE("LayerStack update | Application::run()");

					for (Layer* layer : m_layerStack)
						layer->on_update(timestep);
				}
			}

			if(true)
			{
				HDR_PROFILE_SCOPE("ImGui LayerStack update | Application::run()");

				m_imGuiLayer->begin();
				for (Layer* layer : m_layerStack)
					layer->on_imgui_render();
				m_imGuiLayer->end();
			}


			m_window->on_update();
		}
	}
}