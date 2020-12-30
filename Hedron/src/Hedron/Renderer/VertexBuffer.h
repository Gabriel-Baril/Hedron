#pragma once

namespace Hedron
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static VertexBuffer* create(float* vertices, uint32_t size);
	};
}