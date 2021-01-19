#include "hdrpch.h"

#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Hedron
{
	OpengGLVertexBuffer::OpengGLVertexBuffer(float* vertices, size_t size)
	{
		HDR_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpengGLVertexBuffer::OpengGLVertexBuffer(size_t size)
	{
		HDR_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpengGLVertexBuffer::~OpengGLVertexBuffer()
	{
		HDR_PROFILE_FUNCTION();
		
		glDeleteBuffers(1, &m_rendererID);
	}

	void OpengGLVertexBuffer::bind() const
	{
		HDR_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
	}

	void OpengGLVertexBuffer::unbind() const
	{
		HDR_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpengGLVertexBuffer::set_data(const void* data, size_t size)
	{
		HDR_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}
}