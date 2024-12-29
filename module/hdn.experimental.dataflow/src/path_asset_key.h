#pragma once

#include "mapper.h"
#include "stb_image/stb_image.h"

namespace hdn
{

	struct path_asset_key
	{
		enum Type
		{
			Image
		};

		const char* path;
		Type type;
	};

	template<>
	struct key_resource_mapper<path_asset_key>
	{
		template<typename Func>
		static void get(const path_asset_key& key, const Func& func)
		{
			// Load image from path
			switch (key.type)
			{
			case path_asset_key::Image:
				int width, height, channels;
				unsigned char* data = stbi_load(key.path, &width, &height, &channels, 0);
				func(static_cast<void*>(&data), width * height);
				stbi_image_free(data);
			}
		}
	};
}