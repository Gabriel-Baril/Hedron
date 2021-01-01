#pragma once

namespace Hedron
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void bind(uint32_t slot = 0) const = 0;

		virtual uint32_t get_width() const = 0;
		virtual uint32_t get_height() const = 0;
	};
}