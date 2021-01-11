#pragma once
#include "Hedron/Core/Core.h"


namespace Hedron
{
	struct FrameBufferSpec
	{
		uint32_t width, height;
		uint32_t samples = 1;
		bool swapChainTarget = false; // glBindFramebuffer(0), are we rendering on the screen or not ?
	};

	// 1. Can be used to simply render a scene
	// 2. Post effect pipeline
	// 3. Shadow map
	class FrameBuffer
	{
	public:
		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t get_color_attachment_rendererID() const = 0;

		virtual const FrameBufferSpec& get_spec() const = 0;
		virtual FrameBufferSpec& get_spec() = 0;

		static Ref<FrameBuffer> create(const FrameBufferSpec& spec);
	};
}