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

		virtual void add_vertex_buffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void set_index_buffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& get_vertex_buffers() const override { return m_vertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& get_index_buffer() const override { return m_indexBuffer; }
	private:
		uint32_t m_rendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
	};
}