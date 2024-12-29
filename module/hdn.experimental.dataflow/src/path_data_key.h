#pragma once

#include "key_mapper.h"
#include "image_loader.h"

namespace hdn
{
	struct path_data_key
	{
		enum Type
		{
			Auto,
			Image
		};

		path_data_key(const char* path)
			: path{ path }
		{
		}

		path_data_key(const char* path, Type type)
			: path{ path }, type{ type }
		{
		}

		const char* path = nullptr;
		Type type = Type::Auto;
	};

	template<>
	struct key_mapper<path_data_key>
	{
		static void get(const path_data_key& key, DataResult& out)
		{
			// Load image from path
			switch (key.type)
			{
			case path_data_key::Image:
			{
				Ref<Image> image = ImageRegistry::Get().Get(key.path);
				if (!image)
				{
					image = CreateRef<Image>();
					image->Load(key.path);
					ImageRegistry::Get().Register(key.path, image);
				}
				out.payload = image.get();
				out.payloadByteSize = sizeof(image.get());
				out.found = true;
				break;
			}
			default:
				break;
			}
		}
	};
}