#include "hdrpch.h"
#include "OpenGLTexture2D.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Hedron
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath)
		: m_filePath(filePath)
	{
		int width;
		int height;
		int channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* textureData = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
	
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
		
		
		HDR_CORE_ASSERT(internalFormat && dataFormat, "Format not supported !")

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, internalFormat, m_width, m_height); // We interpret the image has a GL_RGB8, but the image can be RGBA

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // What happen when the texture is smaller than it's size, we do linear interpolation
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // What happen when the texture is bigger than it's size, we do nearest interpolation
		
		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, textureData);

		stbi_image_free(textureData); // We no longer need to store texture data on the CPU
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_rendererID);
	}

	void OpenGLTexture2D::bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_rendererID);
	}
}