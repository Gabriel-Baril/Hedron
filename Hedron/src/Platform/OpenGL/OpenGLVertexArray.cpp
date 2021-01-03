#include "hdrpch.h"

#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hedron
{
	static GLenum shader_data_type_to_opengl_base_type(ShaderDataType type)
	{
		switch (type)
		{
			case Hedron::ShaderDataType::FLOAT:  return GL_FLOAT;
			case Hedron::ShaderDataType::FLOAT2: return GL_FLOAT;
			case Hedron::ShaderDataType::FLOAT3: return GL_FLOAT;
			case Hedron::ShaderDataType::FLOAT4: return GL_FLOAT;
			case Hedron::ShaderDataType::MAT3:   return GL_FLOAT;
			case Hedron::ShaderDataType::MAT4:   return GL_FLOAT;
			case Hedron::ShaderDataType::INT:    return GL_INT;
			case Hedron::ShaderDataType::INT2:   return GL_INT;
			case Hedron::ShaderDataType::INT3:   return GL_INT;
			case Hedron::ShaderDataType::INT4:   return GL_INT;
			case Hedron::ShaderDataType::BOOL:   return GL_BOOL;
		}

		HDR_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_rendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_rendererID);
	}

	void OpenGLVertexArray::bind() const
	{
		glBindVertexArray(m_rendererID);
	}

	void OpenGLVertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::add_vertex_buffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_rendererID); // We can just do this->bind();
		vertexBuffer->bind();

		HDR_CORE_ASSERT(vertexBuffer->get_layout().get_elements().size(), "Vertex has no layout !");

		uint32_t index = 0;
		const BufferLayout& bufferLayout = vertexBuffer->get_layout();
		for (const auto& element : bufferLayout)
		{
			glEnableVertexAttribArray(index);
#pragma warning(push)
#pragma warning(disable:4312)
			glVertexAttribPointer(
				index,
				element.get_component_count(),
				shader_data_type_to_opengl_base_type(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				bufferLayout.get_stride(),
				(const void*)element.offset
			);
#pragma warning(pop)
			index++;
		}

		m_vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::set_index_buffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_rendererID); // We can just do this->bind();
		indexBuffer->bind();

		m_indexBuffer = indexBuffer;
	}
}


