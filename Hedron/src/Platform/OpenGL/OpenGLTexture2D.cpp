#include "hdrpch.h"

#include <stb_image.h>

#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Hedron
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath)
		: m_filePath(filePath)
	{
		HDR_PROFILE_FUNCTION();

		int width;
		int height;
		int channels;

		stbi_uc* textureData = nullptr;
		{
			HDR_PROFILE_SCOPE("stbi_load : OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			stbi_set_flip_vertically_on_load(true);
			textureData = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
		}

	
		HDR_CORE_ASSERT(textureData, "Failed to load Texture2D [OpenGL]");
	
		m_width = width;
		m_height = height;

		GLenum internalFormat = 0;
		GLenum dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		
		m_internalFormat = internalFormat;
		m_dataFormat = dataFormat;
		
		HDR_CORE_ASSERT(internalFormat && dataFormat, "Format not supported !");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, internalFormat, m_width, m_height); // We interpret the image has a GL_RGB8, but the image can be RGBA

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // What happen when the texture is smaller than it's size, we do linear interpolation
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // What happen when the texture is bigger than it's size, we do nearest filtering

		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT); // When we scale the texture coordinate it should scale the textures
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT); // When we scale the texture coordinate it should scale the textures

		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, textureData);

		stbi_image_free(textureData); // We no longer need to store texture data on the CPU
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_width(width), m_height(height)
	{
		HDR_PROFILE_FUNCTION();

		m_internalFormat = GL_RGBA8;
		m_dataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height); // We interpret the image has a GL_RGB8, but the image can be RGBA

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // What happen when the texture is smaller than it's size, we do linear interpolation
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // What happen when the texture is bigger than it's size, we do nearest filtering


		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT); // When we scale the texture coordinate it should scale the textures
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT); // When we scale the texture coordinate it should scale the textures
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		HDR_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_rendererID);
	}

	void OpenGLTexture2D::bind(uint32_t slot) const
	{
		HDR_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_rendererID);
	}

	void OpenGLTexture2D::unbind() const
	{
		HDR_PROFILE_FUNCTION();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLTexture2D::set_data(void* data, uint32_t size)
	{
		HDR_PROFILE_FUNCTION();

		uint32_t bpp = (m_dataFormat == GL_RGBA) ? 4 : 3; // Byte per pixel
		HDR_CORE_ASSERT(size == m_width * m_height * bpp, "Invalid buffer size at line : {0}", __LINE__)
		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
	}
}