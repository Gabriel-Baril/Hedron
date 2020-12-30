#include "hdrpch.h"
#include "OpenGLIndexBuffer.h"

#include <glad/glad.h>

namespace Hedron
{
	OpengGLIndexBuffer::OpengGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_count(count)
	{
		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpengGLIndexBuffer::~OpengGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_rendererID);
	}

	void OpengGLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
	}

	void OpengGLIndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}