#pragma once
#include "Hedron/Renderer/VertexBuffer.h"

namespace Hedron
{
	class OpengGLVertexBuffer : public VertexBuffer
	{
	public:
		OpengGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpengGLVertexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;
	private:
		uint32_t m_rendererID;
	};
}