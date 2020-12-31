#pragma once
#include "Hedron/Renderer/VertexArray.h"

namespace Hedron
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void add_vertex_buffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void set_index_buffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& get_vertex_buffers() const override { return m_vertexBuffers; }
		virtual const Ref<IndexBuffer>& get_index_buffer() const override { return m_indexBuffer; }
	private:
		uint32_t m_rendererID;
		std::vector<Ref<VertexBuffer>> m_vertexBuffers;
		Ref<IndexBuffer> m_indexBuffer;
	};
}