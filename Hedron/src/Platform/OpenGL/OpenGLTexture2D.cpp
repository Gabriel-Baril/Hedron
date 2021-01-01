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

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, GL_RGB8, m_width, m_height);

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // What happen when the texture is smaller than it's size, we do linear interpolation
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // What happen when the texture is bigger than it's size, we do nearest interpolation
		
		if(channels == 3)
			glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		else if (channels == 4)
			glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		else
			HDR_CORE_ASSERT(false, "Unknown image channel count !")

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