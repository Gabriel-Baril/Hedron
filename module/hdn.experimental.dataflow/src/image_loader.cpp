#include "image_loader.h"

#include <stb_image/stb_image.h>
#include <xxhash/xxhash.h>

namespace hdn
{
	void Image::Load(const char* path)
	{
		int width, height, channels;
		m_Pixels = stbi_load(path, &width, &height, &channels, 0);
		m_Width = width;
		m_Height = height;
		m_Channels = channels;
	}

	void Image::Unload()
	{
		if (m_Pixels == nullptr)
		{
			return;
		}
		stbi_image_free(m_Pixels);
	}

	Image::~Image()
	{
		Unload();
	}


	ImageRegistry& ImageRegistry::Get()
	{
		static ImageRegistry s_Instance;
		return s_Instance;
	}

	bool ImageRegistry::Contains(defid_t name)
	{
		size_t length = strlen(name);
		u64 hash = XXH64(name, length, 0);
		return m_ImageRegistry.contains(hash);
	}

	void ImageRegistry::Register(defid_t name, Ref<Image> image)
	{
		size_t length = strlen(name);
		u64 hash = XXH64(name, length, 0); // TODO: Find better seed than 0
		if (m_ImageRegistry.contains(hash))
		{
			return;
		}
		m_ImageRegistry[hash] = image;
	}

	Ref<Image> ImageRegistry::Get(defid_t name)
	{
		size_t length = strlen(name);
		u64 hash = XXH64(name, length, 0); // TODO: Find better seed than 0
		if (m_ImageRegistry.contains(hash))
		{
			return m_ImageRegistry.at(hash);
		}
		return nullptr;
	}
}
