#include "hdrpch.h"

#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLIndexBuffer.h"


namespace Hedron
{
	OpengGLIndexBuffer::OpengGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_count(count)
	{
		HDR_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpengGLIndexBuffer::~OpengGLIndexBuffer()
	{
		HDR_PROFILE_FUNCTION();
		
		glDeleteBuffers(1, &m_rendererID);
	}

	void OpengGLIndexBuffer::bind() const
	{
		HDR_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
	}

	void OpengGLIndexBuffer::unbind() const
	{
		HDR_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}