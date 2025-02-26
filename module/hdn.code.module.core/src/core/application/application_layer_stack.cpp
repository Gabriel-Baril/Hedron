#include "core/profiler/profiler.h"

#include "application_layer_stack.h"

namespace hdn
{
	ApplicationLayerStack::ApplicationLayerStack()
	{
	}

	ApplicationLayerStack::~ApplicationLayerStack()
	{
	}

	void ApplicationLayerStack::PushLayer( IApplicationLayer *layer )
	{
		HDR_PROFILE_FUNCTION();

		m_Layers.emplace( m_Layers.begin() + m_layer_insert_index, layer ); // Added after the last layer
		m_layer_insert_index++;
	}

	void ApplicationLayerStack::PushOverlay( IApplicationLayer *overlay )
	{
		HDR_PROFILE_FUNCTION();

		m_Layers.emplace_back( overlay ); // Added at the back of the list
	}

	void ApplicationLayerStack::PopLayer( IApplicationLayer *layer )
	{
		HDR_PROFILE_FUNCTION();

		auto it = std::find( m_Layers.begin(), m_Layers.end(), layer );
		if ( it != m_Layers.end() )
		{
			layer->OnDetach();
			m_Layers.erase( it );
			m_layer_insert_index--;
		}
	}

	void ApplicationLayerStack::PopOverlay( IApplicationLayer *overlay )
	{
		HDR_PROFILE_FUNCTION();

		auto it = std::find( m_Layers.begin(), m_Layers.end(), overlay );
		if ( it != m_Layers.end() )
		{
			overlay->OnDetach();
			m_Layers.erase( it );
		}
	}
}
