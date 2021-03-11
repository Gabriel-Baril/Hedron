#pragma once
#include "Hedron/Core/Core.h"
#include <Hedron.h>


namespace Hedron
{
	enum class FrameBufferTextureFormat
	{
		NONE = 0,
		
		// Color
		RGBA8,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		DEPTH = DEPTH24STENCIL8
	};

	// Represent each individual texture that we add to our framebuffer
	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format)
			: textureFormat(format) {}

		FrameBufferTextureFormat textureFormat = FrameBufferTextureFormat::NONE;
		// TODO: filtering/wrap
	};

	// Contains a list of texture specification
	struct FrameBufferAttachmentSpecification
	{
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
			: attachments{ attachments } 
		{
		}

		std::vector<FrameBufferTextureSpecification> attachments;
	};

	struct FrameBufferSpecification
	{
		uint32_t width;
		uint32_t height;
		FrameBufferAttachmentSpecification attachments;
		uint32_t samples = 1;
		bool swapChainTarget = false; // glBindFramebuffer(0), are we rendering on the screen or not ?
	};

	// 1. Can be used to simply render a scene
	// 2. Post effect pipeline
	// 3. Shadow map
	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t get_color_attachment_rendererID(uint32_t index = 0) const = 0;

		virtual const FrameBufferSpecification& get_spec() const = 0;
		virtual FrameBufferSpecification& get_spec() = 0;

		static Ref<FrameBuffer> create(const FrameBufferSpecification& spec);
	};
}