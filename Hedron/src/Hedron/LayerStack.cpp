#include "hdrpch.h"
#include "LayerStack.h"

namespace Hedron
{

	LayerStack::LayerStack()
	{
		m_layerInsert = m_layers.begin();
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
		m_layerInsert = m_layers.emplace(m_layerInsert, layer); // Added after the last layer
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
			m_layers.erase(it);
			m_layerInsert--;
		}
	}

	void LayerStack::pop_overlay(Layer* overlay)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
		}
	}
}