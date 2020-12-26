#pragma once

#include "Hedron/Core.h"
#include "Layer.h"

#include <vector>

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
	private:
		std::vector<Layer*> m_layers; // We need to use a vector and not a stack because we need to reverse it and push elements in the middle
		std::vector<Layer*>::iterator m_layerInsert;
	};
}