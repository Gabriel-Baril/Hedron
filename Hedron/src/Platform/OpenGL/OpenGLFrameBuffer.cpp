#include "hdrpch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Hedron
{
	static const uint32_t s_maxFrameBufferSize = 8192;
	
	
	namespace Util
	{
		static GLenum texture_target(bool multiSampled)
		{
			return multiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void create_textures(bool multiSampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(texture_target(multiSampled), count, outID);
		}

		static void bind_texture(bool multiSampled, uint32_t id)
		{
			glBindTexture(texture_target(multiSampled), id);
		}

		static void attach_color_texture(uint32_t id, int samples, GLenum format, uint32_t width, uint32_t height, uint32_t index)
		{
			bool multiSampled = samples > 1;
			if (multiSampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				// internalFormat : How OpenGL store the data, Format: How we access the data
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture_target(multiSampled), id, 0); // Attach our created texture to our framebuffer
		}

		static void attach_depth_texture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multiSampled = samples > 1;
			if (multiSampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				// internalFormat : How OpenGL store the data, Format: How we access the data
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, texture_target(multiSampled), id, 0);
		}

		static bool is_depth_format(FrameBufferTextureFormat format)
		{
			switch (format)
			{
				case FrameBufferTextureFormat::DEPTH24STENCIL8: return true;
			}
			return false;
		}
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
		: m_frameBufferSpec(spec)
	{
		for (auto& spec : m_frameBufferSpec.attachments.attachments)
		{
			if (Util::is_depth_format(spec.textureFormat))
				m_depthAttachmentSpecification = spec;
			else
				m_colorAttachmentSpecifications.emplace_back(spec);
		}

		invalidate();
	}


	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_rendererID);
		glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
		glDeleteTextures(1, &m_depthAttachment);
	}

	// Reset our framebuffer to a valid state
	void OpenGLFrameBuffer::invalidate()
	{
		if (m_rendererID)
		{
			glDeleteFramebuffers(1, &m_rendererID);
			glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
			glDeleteTextures(1, &m_depthAttachment);

			m_colorAttachments.clear();
			m_depthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

		bool multiSampled = m_frameBufferSpec.samples > 1;
		
		// Attachments
		if (m_colorAttachmentSpecifications.size() > 0)
		{
			m_colorAttachments.resize(m_colorAttachmentSpecifications.size());
			Util::create_textures(multiSampled, m_colorAttachments.data(), m_colorAttachments.size());

			for (size_t i = 0; i < m_colorAttachments.size(); i++)
			{
				Util::bind_texture(multiSampled, m_colorAttachments[i]);
				switch (m_colorAttachmentSpecifications[i].textureFormat)
				{
					case FrameBufferTextureFormat::RGBA8:
						Util::attach_color_texture(m_colorAttachments[i], m_frameBufferSpec.samples, GL_RGBA8, m_frameBufferSpec.width, m_frameBufferSpec.height, i);
						break;
				}
			}
		}

		if (m_depthAttachmentSpecification.textureFormat != FrameBufferTextureFormat::NONE)
		{
			Util::create_textures(multiSampled, &m_depthAttachment, 1);
			Util::bind_texture(multiSampled, m_depthAttachment);

			switch (m_depthAttachmentSpecification.textureFormat)
			{
				case FrameBufferTextureFormat::DEPTH24STENCIL8:
					Util::attach_depth_texture(m_depthAttachment, m_frameBufferSpec.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_frameBufferSpec.width, m_frameBufferSpec.height);
					break;
			}
		}

		if (m_colorAttachments.size() > 1)
		{
			HDR_CORE_ASSERT(m_colorAttachments.size() <= 4, "");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(4, buffers);
		}
		else if(m_colorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		HDR_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
		glViewport(0, 0, m_frameBufferSpec.width, m_frameBufferSpec.height);
	}

	void OpenGLFrameBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_maxFrameBufferSize || height > s_maxFrameBufferSize)
		{
			HDR_CORE_WARNING("[{0}, {1}] is an invalid framebuffer size", width, height);
			return;
		}
		m_frameBufferSpec.width = width;
		m_frameBufferSpec.height = height;
		this->invalidate();
	}

}