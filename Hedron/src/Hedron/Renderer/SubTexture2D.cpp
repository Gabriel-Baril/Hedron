#include "hdrpch.h"
#include "SubTexture2D.h"

namespace Hedron
{

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_texture(texture)
	{
		m_texCoords[0] = { min.x, min.y };
		m_texCoords[1] = { max.x, min.y };
		m_texCoords[2] = { max.x, max.y };
		m_texCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::create_from_coords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2 cellSize, const glm::vec2& spriteDimension)
	{
		//
		/*
		auto pixelMap = [sheetHeight, spriteHeight](uint32_t x, uint32_t y) {return glm::vec2{ x, -spriteHeight + sheetHeight - y }; };
		auto pixelSpriteMap = [&pixelMap, spriteWidth, spriteHeight](uint32_t x, uint32_t y) {return pixelMap((spriteWidth + 1) * x + 1, (spriteHeight + 1) * y + 1); };
		auto pixelSpriteMapNorm = [&pixelSpriteMap, sheetWidth, sheetHeight](uint32_t x, uint32_t y)
		{
			auto spriteCoord = pixelSpriteMap(x, y);
			return glm::vec2{ spriteCoord.x / sheetWidth, spriteCoord.y / sheetHeight };
		};
		*/

		glm::vec2 min = { (coords.x * cellSize.x) / (float)texture->get_width(), (coords.y * cellSize.y) / (float)texture->get_height() };
		glm::vec2 max = { ((coords.x + spriteDimension.x) * cellSize.x) / (float)texture->get_width(), ((coords.y + spriteDimension.y) * cellSize.y) / (float)texture->get_height() };

		return create_ref<SubTexture2D>(texture, min, max);
	}
}

