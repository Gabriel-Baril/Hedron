#pragma once

#include "application_layer.h"
#include "application_layer_stack.h"
#include "../event/event.h"
#include "../event/event_dispatcher.h"

namespace dm
{
	struct ApplicationConfig
	{
		u32 windowWidth;
		u32 windowHeight;
		const char *applicationName;
	};

	class IApplication
	{
	public:
		template <typename T, typename... Args>
		IApplicationLayer *register_layer(Args &&...args)
		{
			IApplicationLayer *layer = create_ref<T>(std::forward<Args>(args)...);
			m_ApplicationLayerStack.layer_push(layer);
			return layer;
		}

		template <typename T>
		void unregister_layer()
		{
			// TODO: Unregister layer of the same type (The layer type is defined by the class type)
		}

		void unregister_layer(int id)
		{
			// TODO: Unregister layer by it's id
		}

		void on_update(float dt);
		virtual void on_event(Event &event);
		void run();

	private:
		ApplicationLayerStack m_ApplicationLayerStack;
		// Other global application-wide information (name, id, config, etc)
	};
}
