#include "image_loader.h"

#include "core/hash.h"

#include <stb_image/stb_image.h>

namespace hdn
{
	void Image::load(const char* path)
	{
		int width, height, channels;
		m_Pixels = stbi_load(path, &width, &height, &channels, 0);
		m_Width = width;
		m_Height = height;
		m_Channels = channels;
	}

	void Image::unload()
	{
		if (m_Pixels == nullptr)
		{
			return;
		}
		stbi_image_free(m_Pixels);
	}

	Image::~Image()
	{
		unload();
	}


	ImageRegistry& ImageRegistry::get()
	{
		static ImageRegistry s_Instance;
		return s_Instance;
	}

	bool ImageRegistry::contains(const char* name)
	{
		size_t length = strlen(name);
		u64 hash = hash_generate(name);
		return m_ImageRegistry.contains(hash);
	}

	void ImageRegistry::register_image(const char* name, Ref<Image> image)
	{
		size_t length = strlen(name);
		u64 hash = hash_generate(name);
		if (m_ImageRegistry.contains(hash))
		{
			return;
		}
		m_ImageRegistry[hash] = image;
	}

	Ref<Image> ImageRegistry::get(const char* name)
	{
		size_t length = strlen(name);
		u64 hash = hash_generate(name);
		if (m_ImageRegistry.contains(hash))
		{
			return m_ImageRegistry.at(hash);
		}
		return nullptr;
	}
}
