#pragma once

#include "core/stl/vector.h"
#include "application_layer.h"

namespace hdn
{
	class ApplicationLayerStack
	{
	public:
		ApplicationLayerStack();
		~ApplicationLayerStack();

		void layer_push( IApplicationLayer *layer );
		void overlay_push( IApplicationLayer *overlay );
		void layer_pop( IApplicationLayer *layer );
		void overlasy_pop( IApplicationLayer *overlay );

		vector<IApplicationLayer*>::iterator begin() { return m_Layers.begin(); }
		vector<IApplicationLayer*>::iterator end() { return m_Layers.end(); }
		vector<IApplicationLayer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		vector<IApplicationLayer*>::reverse_iterator rend() { return m_Layers.rend(); }

		vector<IApplicationLayer*>::const_iterator begin() const { return m_Layers.begin(); }
		vector<IApplicationLayer*>::const_iterator end() const { return m_Layers.end(); }
		vector<IApplicationLayer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		vector<IApplicationLayer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }
	private:
		vector<IApplicationLayer*> m_Layers; // We need to use a vector and not a stack because we need to reverse it and push elements in the middle
		unsigned int m_layer_insert_index = 0;
	};
}