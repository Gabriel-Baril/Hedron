#pragma once
#include "Hedron/Renderer/VertexBuffer.h"

namespace Hedron
{
	class OpengGLVertexBuffer : public VertexBuffer
	{
	public:
		OpengGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpengGLVertexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;
	
		virtual inline void set_layout(const BufferLayout& layout) override { m_layout = layout; };
		virtual inline const BufferLayout& get_layout() const override { return m_layout; };
	private:
		uint32_t m_rendererID;
		BufferLayout m_layout;
	};
}