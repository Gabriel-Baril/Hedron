#include "application.h"

namespace dm
{
	void IApplication::on_update(float dt)
	{
		for (const auto &layer : m_ApplicationLayerStack)
		{
			if (!layer->enabled())
			{
				continue;
			}
			layer->begin();
			layer->on_update(dt);
			layer->end();
		}
	}

	void IApplication::on_event(Event &event)
	{
		EventDispatcher dispatcher(event);
		// dispatcher.Dispatch<WindowClosedEvent>( HDR_BIND_EVENT_FN( Application::OnWindowClosed ) );
		// dispatcher.Dispatch<WindowResizedEvent>( HDR_BIND_EVENT_FN( Application::OnWindowResized ) );
		// dispatcher.Dispatch<KeyTypedEvent>( HDR_BIND_EVENT_FN( Application::OnKeyTyped ) );
		// dispatcher.Dispatch<KeyReleasedEvent>( HDR_BIND_EVENT_FN( Application::OnKeyReleased ) );

		for (auto it = m_ApplicationLayerStack.end(); it != m_ApplicationLayerStack.begin();)
		{
			if (event.Handled)
				break;
			(*--it)->on_event(event);
		}
	}

	void IApplication::run()
	{
		while (true)
		{
			float dt = 0; // TOOD: Compute deltatime
			on_update(dt);
		}
	}
}
