#pragma once

#include <vector>

#include "Hedron/Core/Core.h"
#include "Hedron/Core/Layer.h"

namespace Hedron
{
	// Wrapper class used to manage a vector of layers
	class HEDRON_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void push_layer(Layer* layer);
		void push_overlay(Layer* overlay);
		void pop_layer(Layer* layer);
		void pop_overlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return m_layers.begin(); }
		std::vector<Layer*>::const_iterator end() const { return m_layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_layers.rend(); }
	private:
		std::vector<Layer*> m_layers; // We need to use a vector and not a stack because we need to reverse it and push elements in the middle
		unsigned int m_layerInsertIndex = 0;
	};
}