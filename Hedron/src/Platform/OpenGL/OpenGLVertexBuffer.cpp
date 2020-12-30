#include "hdrpch.h"
#include "OpenGLVertexBuffer.h"

#include <glad/glad.h>

namespace Hedron
{
	OpengGLVertexBuffer::OpengGLVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpengGLVertexBuffer::~OpengGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_rendererID);
	}

	void OpengGLVertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
	}

	void OpengGLVertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}