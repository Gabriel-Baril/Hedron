#include "hdrpch.h"

#include "Hedron/Core/LayerStack.h"

namespace Hedron
{

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_layers)
		{
			delete layer;
		}
	}

	void LayerStack::push_layer(Layer* layer)
	{
		m_layers.emplace(m_layers.begin() + m_layerInsertIndex, layer); // Added after the last layer
		m_layerInsertIndex++;
	}

	void LayerStack::push_overlay(Layer* overlay)
	{
		m_layers.emplace_back(overlay); // Added at the back of the list
	}

	void LayerStack::pop_layer(Layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it != m_layers.end())
		{
			layer->on_detach();
			m_layers.erase(it);
			m_layerInsertIndex--;
		}
	}

	void LayerStack::pop_overlay(Layer* overlay)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
		if (it != m_layers.end())
		{
			overlay->on_detach();
			m_layers.erase(it);
		}
	}
}