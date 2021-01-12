#pragma once

namespace Hedron
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void init() = 0;
		virtual void swap_buffers() = 0;
	};
}