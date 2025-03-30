#pragma once

#include "core/core.h"
#include "core/stl/unordered_map.h"

namespace hdn
{
	class Image
	{
	public:
		void load(const char* path);
		void unload();
		virtual ~Image();

		u32 get_width() { return m_Width; }
		u32 get_height() { return m_Height; }
		u32 get_channel_count() { return m_Channels; }
	private:
		void* m_Pixels = nullptr;
		u32 m_Width;
		u32 m_Height;
		u32 m_Channels;
	};

	class ImageRegistry
	{
	public:
		static ImageRegistry& get();

		bool contains(const char* name);
		void register_image(const char* name, Ref<Image> image);
		Ref<Image> get(const char* name);
	private:
		ImageRegistry() = default;
	private:
		unordered_map<u64, Ref<Image>> m_ImageRegistry{};
	};
}