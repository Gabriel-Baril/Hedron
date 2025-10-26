#pragma once

#include "core/core.h"

namespace hdn
{
	struct Vertex
	{
		vec3f32 position{};
		vec3f32 color{};
		vec3f32 normal{};
		vec2f32 uv{};

		bool operator==(const Vertex& other) const
		{
			return
				position == other.position &&
				color == other.color &&
				normal == other.normal &&
				uv == other.uv;
		}
	};

	struct ModelAsset
	{
		u32 vertexCount;
		u32 indexCount;
		Vertex* vertices;
		u32* indices;
	};
}