#pragma once

#include "application_layer.h"
#include "application_layer_stack.h"
#include "../event/event.h"
#include "../event/event_dispatcher.h"

namespace hdn
{
	class IApplication
	{
	public:
		template<typename T, typename... Args>
		IApplicationLayer *Register( Args&&... args )
		{
			IApplicationLayer *layer = CreateRef<T>( std::forward<Args>( args )... );
			m_ApplicationLayerStack.PushLayer( layer );
			return layer;
		}

		template<typename T>
		void Unregister()
		{
			// TODO: Unregister layer of the same type (The layer type is defined by the class type)
		}

		void Unregister( int id )
		{
			// TODO: Unregister layer by it's id
		}

		void Update( float dt )
		{
			for ( const auto &layer : m_ApplicationLayerStack)
			{
				if ( !layer->Enabled() )
				{
					continue;
				}
				layer->Begin();
				layer->OnUpdate( dt );
				layer->End();
			}
		}

		virtual void OnEvent(Event& event)
		{
			EventDispatcher dispatcher( event );
			// dispatcher.Dispatch<WindowClosedEvent>( HDR_BIND_EVENT_FN( Application::OnWindowClosed ) );
			// dispatcher.Dispatch<WindowResizedEvent>( HDR_BIND_EVENT_FN( Application::OnWindowResized ) );
			// dispatcher.Dispatch<KeyTypedEvent>( HDR_BIND_EVENT_FN( Application::OnKeyTyped ) );
			// dispatcher.Dispatch<KeyReleasedEvent>( HDR_BIND_EVENT_FN( Application::OnKeyReleased ) );

			for ( auto it = m_ApplicationLayerStack.end(); it != m_ApplicationLayerStack.begin(); )
			{
				if ( event.Handled )
					break;
				(*--it)->OnEvent( event );
			}
		}

		void Run()
		{
			while ( true )
			{
				float dt = 0; // TOOD: Compute deltatime
				Update( dt );
			}
		}
	private:
		ApplicationLayerStack m_ApplicationLayerStack;
		// Other global application-wide information (name, id, config, etc)
	};
}