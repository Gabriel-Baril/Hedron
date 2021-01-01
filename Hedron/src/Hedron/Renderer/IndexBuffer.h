#pragma once

namespace Hedron
{
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t get_count() const = 0;

		static Ref<IndexBuffer> create(uint32_t* indices, uint32_t size);
	};
}