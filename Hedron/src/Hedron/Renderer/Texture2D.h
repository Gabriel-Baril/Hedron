#pragma once

#include <string>

#include "Hedron/Core/Core.h"

#include "Hedron/Renderer/Texture.h"

namespace Hedron
{
	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		virtual void bind(uint32_t slot = 0) const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t get_width() const = 0;
		virtual uint32_t get_height() const = 0;

		virtual uint32_t get_renderer_id() const = 0;

		virtual bool operator==(const Texture& other) const = 0;

		static Ref<Texture2D> create(uint32_t width, uint32_t height);
		static Ref<Texture2D> create(const std::string& filePath);
	};
}