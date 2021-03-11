#pragma once
#include "Hedron/Renderer/FrameBuffer.h"

namespace Hedron
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		virtual const FrameBufferSpecification& get_spec() const { return m_frameBufferSpec; };
		virtual FrameBufferSpecification& get_spec() { return m_frameBufferSpec; };
	
		void invalidate();
		void bind() override;
		void unbind() override;

		virtual void resize(uint32_t width, uint32_t height) override;

		virtual uint32_t get_color_attachment_rendererID(uint32_t index = 0) const override
		{
			HDR_CORE_ASSERT(index < m_colorAttachments.size(), "Index array out of bound!")
			return m_colorAttachments[index];
		};
	private:
		uint32_t m_rendererID = 0;
		FrameBufferSpecification m_frameBufferSpec;

		std::vector<FrameBufferTextureSpecification> m_colorAttachmentSpecifications;
		FrameBufferTextureSpecification m_depthAttachmentSpecification = FrameBufferTextureFormat::NONE;
	
		std::vector<uint32_t> m_colorAttachments; // {};
		uint32_t m_depthAttachment = 0; // RendererIDs
	};
}