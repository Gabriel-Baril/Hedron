#pragma once

#include "Hedron/Renderer/BufferLayout.h"

namespace Hedron
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void set_layout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& get_layout() const = 0;

		static Ref<VertexBuffer> create(float* vertices, uint32_t size);
	};
}