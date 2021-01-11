#pragma once
#include "Hedron/Renderer/FrameBuffer.h"

namespace Hedron
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpec& spec);
		virtual ~OpenGLFrameBuffer();

		virtual const FrameBufferSpec& get_spec() const { return m_frameBufferSpec; };
		virtual FrameBufferSpec& get_spec() { return m_frameBufferSpec; };
	
		void invalidate();
		void bind() override;
		void unbind() override;

		virtual uint32_t get_color_attachment_rendererID() const override { return m_colorAttachment; };
	private:
		uint32_t m_rendererID;
		uint32_t m_colorAttachment, m_depthAttachment;
		FrameBufferSpec m_frameBufferSpec;
	};
}