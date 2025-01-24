#include "image_loader.h"

#include "core/hash.h"

#include <stb_image/stb_image.h>

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

	bool ImageRegistry::Contains(const char* name)
	{
		size_t length = strlen(name);
		u64 hash = GenerateHash(name);
		return m_ImageRegistry.contains(hash);
	}

	void ImageRegistry::Register(const char* name, Ref<Image> image)
	{
		size_t length = strlen(name);
		u64 hash = GenerateHash(name);
		if (m_ImageRegistry.contains(hash))
		{
			return;
		}
		m_ImageRegistry[hash] = image;
	}

	Ref<Image> ImageRegistry::Get(const char* name)
	{
		size_t length = strlen(name);
		u64 hash = GenerateHash(name);
		if (m_ImageRegistry.contains(hash))
		{
			return m_ImageRegistry.at(hash);
		}
		return nullptr;
	}
}
