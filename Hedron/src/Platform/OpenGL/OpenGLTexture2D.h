#pragma once

#include "Hedron/Renderer/Texture2D.h"
#include <glad/glad.h>

namespace Hedron
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& filePath);
		OpenGLTexture2D(uint32_t width, uint32_t height);

		virtual ~OpenGLTexture2D();

		virtual void bind(uint32_t slot = 0) const override;
		virtual void unbind() const override;

		virtual uint32_t get_width() const override { return m_width; }
		virtual uint32_t get_height() const override { return m_height; }

		virtual void set_data(void* data, uint32_t size) override;

		virtual bool operator==(const Texture& other) const override 
		{
			return m_rendererID == ((OpenGLTexture2D&)other).m_rendererID;
		}
	private:
		std::string m_filePath;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_rendererID;
		GLenum m_internalFormat;
		GLenum m_dataFormat;
	};
}