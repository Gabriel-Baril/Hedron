#pragma once

#include "Hedron/Renderer/IndexBuffer.h"

namespace Hedron
{
	class OpengGLIndexBuffer : public IndexBuffer
	{
	public:
		OpengGLIndexBuffer(uint32_t* indices, uint32_t size);
		virtual ~OpengGLIndexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual uint32_t get_count() const override { return m_count; };
	private:
		uint32_t m_rendererID;
		uint32_t m_count;
	};
}