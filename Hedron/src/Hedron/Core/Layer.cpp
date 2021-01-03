#include "hdrpch.h"

#include "Hedron/Core/Layer.h"

namespace Hedron
{
	Layer::Layer(const std::string& debugName)
		: m_debugName(debugName)
	{
	}

	Layer::~Layer()
	{
	}
}