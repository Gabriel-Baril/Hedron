#pragma once

#include "Hedron/Renderer/Texture2D.h"

namespace Hedron
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& filePath);
		virtual ~OpenGLTexture2D();

		virtual void bind(uint32_t slot = 0) const override;

		virtual uint32_t get_width() const override { return m_width; }
		virtual uint32_t get_height() const override { return m_height; }
	private:
		std::string m_filePath;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_rendererID;
	};
}