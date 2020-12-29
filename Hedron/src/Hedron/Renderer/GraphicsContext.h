#pragma once

namespace Hedron
{
	class GraphicsContext
	{
	public:
		virtual void init() = 0;
		virtual void swap_buffers() = 0;
	};
}