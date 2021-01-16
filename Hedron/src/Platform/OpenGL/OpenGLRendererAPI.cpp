#include "hdrpch.h"

#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLRendererAPI.h"


namespace Hedron
{
	void OpenGLRendererAPI::init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);

		//glEnable(GL_PROGRAM_POINT_SIZE);
	}

	void OpenGLRendererAPI::set_clear_color(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::draw_indexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t indexToDraw = (indexCount == 0) ? vertexArray->get_index_buffer()->get_count() : indexCount;
		
		glDrawElements(GL_TRIANGLES, indexToDraw, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLRendererAPI::draw_line_loop(uint32_t startVertex, uint32_t endVertex)
	{
		glDrawArrays(GL_LINE_LOOP, startVertex, endVertex);
	}

	void OpenGLRendererAPI::set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}
}
