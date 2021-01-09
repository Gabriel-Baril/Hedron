#pragma once

#include <glm/glm.hpp>

#include "Hedron/Renderer/Texture2D.h"

namespace Hedron
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture2D> get_texture() const { return m_texture; }
		const glm::vec2* get_tex_coords() const { return m_texCoords; }

		// sprite dimension is in term of cell size
		// TODO: Add overloads
		static Ref<SubTexture2D> create_from_coords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2 cellSize, const glm::vec2& spriteDimension = { 1.0f, 1.0f });
	private:
		Ref<Texture2D> m_texture;

		glm::vec2 m_texCoords[4];
	};
}