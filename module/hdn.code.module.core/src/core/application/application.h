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
			IApplicationLayer *layer = create_ref<T>( std::forward<Args>( args )... );
			m_ApplicationLayerStack.layer_push( layer );
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

		void on_update(float dt);
		virtual void on_event(Event& event);
		void run();
	private:
		ApplicationLayerStack m_ApplicationLayerStack;
		// Other global application-wide information (name, id, config, etc)
	};
}