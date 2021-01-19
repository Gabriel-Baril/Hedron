#pragma once

#include "Hedron/Core/Core.h"
#include "Hedron/Renderer/BufferLayout.h"

namespace Hedron
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void set_data(const void* data, size_t size) = 0;
		virtual void set_layout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& get_layout() const = 0;

		static Ref<VertexBuffer> create(float* vertices, size_t size);
		static Ref<VertexBuffer> create(size_t size);
	};
}